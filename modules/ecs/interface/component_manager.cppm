module;

#include <typeinfo>

export module ecs.component_manager;

import ecs.common;
import ecs.icomponent;
import ecs.global_memory_user;
import ecs.memory_chunk_allocator;

import types.string;
import types.vector;
import types.unordered_map;
import types.base_types;

namespace gg
{

export class ComponentManager : private memory::GlobalMemoryUser
{
private:
    friend class IComponent;

    class IComponentContainer
    {
    public:
        virtual ~IComponentContainer() = default;
        virtual String getComponentContainerTypeName() const = 0;
        virtual void destroyComponent(IComponent* object) = 0;
    };

    template<typename T>
    class ComponentContainer : public memory::MemoryChunkAllocator<T, COMPONENT_T_CHUNK_SIZE>, public IComponentContainer
    {
    public:
        ComponentContainer()
            : memory::MemoryChunkAllocator<T, COMPONENT_T_CHUNK_SIZE>("ComponentManager")
        {}
        virtual ~ComponentContainer() = default;

        virtual String getComponentContainerTypeName() const override
        {
            static String COMPONENT_TYPE_NAME{typeid(T).name()};
            return COMPONENT_TYPE_NAME;
        }

        virtual void destroyComponent(IComponent* object) override
        {
            object->~IComponent();
            this->destroyObject(object);
        }
    private:
        ComponentContainer(const ComponentContainer&) = delete;
        ComponentContainer& operator=(ComponentContainer&) = delete;
    };

public:
    template<typename T>
    using TComponentIterator = typename ComponentContainer<T>::iterator;

    ComponentManager();
    ~ComponentManager();

    template<typename T, class... ARGS>
    T* addComponent(const EntityId& entity_id, ARGS&&... args)
    {
        static constexpr std::hash<ComponentId> entity_component_id_hasher{std::hash<ComponentId>()};
        const ComponentTypeId component_type_id = T::STATIC_COMPONENT_TYPE_ID;

        void* object_memory = getComponentContainer<T>()->createObject();

        ComponentId component_id = this->aqcuireComponentId((T*)object_memory);
        ((T*)object_memory)->component_id = component_id;

        IComponent* component = new (object_memory) T(std::forward<ARGS>(args)...);
        component->setOwner(entity_id);
        component->setHash(entity_component_id_hasher(entity_id) ^ (entity_component_id_hasher(component_id) << 1));

        mapEntityComponent(entity_id, component_id, component_type_id);

        return static_cast<T*>(component);
    }

    template <typename T>
    void removeComponent(const EntityId entity_id)
    {
        const ComponentTypeId component_type_id = T::STATIC_COMPONENT_TYPE_ID;

        const ComponentId component_id = this->entity_component_map[entity_id.index][component_type_id];

        IComponent* component = this->component_lookup_table[component_id];

        assert(component != nullptr && "FATAL: Trying to remove a component "
                                       "which is not used by this entity!");

        getComponentContainer<T>()->destroyObject(component);

        unmapEntityComponent(entity_id, component_id, component_type_id);
    }

    void removeAllComponents(const EntityId entity_id);

    template <typename T>
    T* getComponent(const EntityId entity_id)
    {
        const ComponentTypeId component_type_id = T::STATIC_COMPONENT_TYPE_ID;

        const ComponentId component_id = this->entity_component_map[entity_id.index][component_type_id];

        if (component_id == INVALID_COMPONENT_ID)
        {
            return nullptr;
        }

        return static_cast<T*>(this->component_lookup_table[component_id]);
    }

    template <typename T>
    inline TComponentIterator<T> begin()
    {
        return getComponentContainer<T>()->begin();
    }

    template <typename T>
    inline TComponentIterator<T> end()
    {
        return getComponentContainer<T>()->end();
    }

private:
    template <typename T>
    inline ComponentContainer<T>* getComponentContainer()
    {
        ComponentTypeId component_type_id = T::STATIC_COMPONENT_TYPE_ID;

        auto it = this->component_container_registry.find(component_type_id);
        ComponentContainer<T>* cc = nullptr;

        if (it == this->component_container_registry.end())
        {
            cc = new ComponentContainer<T>();
            this->component_container_registry[component_type_id] = cc;
        }
        else
        {
            cc = static_cast<ComponentContainer<T>*>(it->second);
        }

        assert(cc != nullptr && "Failed to create ComponentContainer<T>!");
        return cc;
    }

    ComponentId aqcuireComponentId(IComponent* component)
    {
        U32 i = 0;
        for (; i < this->component_lookup_table.size(); ++i)
        {
            if (this->component_lookup_table[i] == nullptr)
            {
                this->component_lookup_table[i] = component;
                return i;
            }
        }

        this->component_lookup_table.resize(this->component_lookup_table.size() + COMPONENT_LUT_GROW, nullptr);

        this->component_lookup_table[i] = component;
        return i;
    }

    void releaseComponentId(ComponentId id);

    void mapEntityComponent(EntityId entity_id, ComponentId component_id, ComponentTypeId component_type_id);

    void unmapEntityComponent(EntityId entity_id, ComponentId component_id, ComponentTypeId component_type_id);


private:
    using ComponentContainerRegistry = UnorderedMap<ComponentTypeId, IComponentContainer*>;
    ComponentContainerRegistry component_container_registry;

    using ComponentLookupTable = Vector<IComponent*>;
    ComponentLookupTable component_lookup_table;

    using EntityComponentMap = Vector<Vector<ComponentId>>;
    EntityComponentMap entity_component_map;
};

} // namespace gg;
