module;

#include <typeinfo>
#include <utility>

export module ecs.entity_manager;

import ecs.ientity;
import ecs.handle;
import ecs.memory_chunk_allocator;
import ecs.common;
import ecs.component_manager;

import types.string;
import types.unordered_map;
import types.vector;
import types.base_types;

namespace gg
{

export class EntityManager
{
private:
    class IEntityContainer
    {
    public:
        virtual ~IEntityContainer() = default;
        [[nodiscard]] virtual String getEntityContainerTypeName() const = 0;
        virtual void destroyEntity(IEntity* object) = 0;
    };

    template<typename T>
    class EntityContainer : public memory::MemoryChunkAllocator<T, ENTITY_T_CHUNK_SIZE>, public IEntityContainer
    {
    public:
        EntityContainer()
            : memory::MemoryChunkAllocator<T, ENTITY_T_CHUNK_SIZE>("EntityManager")
        {}
        virtual ~EntityContainer() = default;

        [[nodiscard]] virtual String getEntityContainerTypeName() const override
        {
            static String ENTITY_TYPE_NAME{typeid(T).name()};
            return ENTITY_TYPE_NAME;
        }

        virtual void destroyEntity(IEntity* object) override
        {
            object->~IEntity();
            this->destroyObject(object);
        }
    };
public:
    EntityManager(ComponentManager* component_manager_instance);
    ~EntityManager();

    template<typename T, class... ARGS>
    [[nodiscard]] EntityId createEntity(ARGS&&... args)
    {
        void* object_memory = getEntityContainer<T>()->createObject();
        EntityId entity_id = this->acquireEntityId((T*)object_memory);
        IEntity* entity = new (object_memory) T(entity_id, this->component_manager, std::forward<ARGS>(args)...);
        return entity_id;
    }

    void destroyEntity(EntityId entity_id);

    [[nodiscard]] inline IEntity* getEntity(EntityId entity_id)
    {
        return this->entity_handle_table[entity_id];
    }

    [[nodiscard]] inline EntityId getEntityId(EntityId::value_type index) const
    {
        return this->entity_handle_table[index];
    }

    void removeDestroyedEntities();

    template<typename T>
    [[nodiscard]] EntityContainer<T>* getEntityContainer()
    {
        EntityTypeId entity_type_id = T::STATIC_ENTITY_TYPE_ID;

        auto it = this->entity_registry.find(entity_type_id);
        EntityContainer<T>* entity_container = nullptr;

        if(it == this->entity_registry.end())
        {
            entity_container = new EntityContainer<T>();
            this->entity_registry[entity_type_id] = entity_container;
        }
        else
        {
            entity_container = (EntityContainer<T>*)it->second;
        }

        assert(entity_container != nullptr && "Failed to create EntityContainer<T>!");
        return entity_container;
    }

    [[nodiscard]] EntityId acquireEntityId(IEntity* entity);

    void releaseEntityId(EntityId entity_id);

private:
    using EntityRegistry = UnorderedMap<EntityTypeId, IEntityContainer*>;
    using PendingDestroyEntities = Vector<EntityId>;
    using EntityHandleTable = util::HandleTable<IEntity, EntityId>;

    EntityRegistry entity_registry;
    PendingDestroyEntities pending_destroy_entities;
    Size num_pending_destroyed_entities;
    ComponentManager* component_manager;
    EntityHandleTable entity_handle_table;

};

} // namespace gg;
