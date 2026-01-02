module;

#include <utility>

export module ecs.system_manager;

import ecs.global_memory_user;
import ecs.common;
import ecs.isystem;
import ecs.linear_allocator;

import types.unordered_map;
import types.base_types;
import types.vector;
import types.list;

namespace gg
{
    
export class SystemManager : private memory::GlobalMemoryUser
{
public:
    SystemManager();
    ~SystemManager();

    template <typename T, class... ARGS>
    [[nodiscard]] T* addSystem(ARGS&&... system_args)
    {
        const U64 static_system_type_id = T::STATIC_SYSTEM_TYPE_ID;

        auto it = this->system_registry.find(static_system_type_id);
        if ((this->system_registry.find(static_system_type_id) != this->system_registry.end()) && (it->second != nullptr))
        {
            return (T*)it->second;
        }

        T* system = nullptr;
        void* system_memory = this->system_allocator->allocate(sizeof(T), alignof(T));
        if (system_memory != nullptr)
        {

            ((T*)system_memory)->system_manager = this;

            system = new (system_memory) T(std::forward<ARGS>(system_args)...);
            this->system_registry[static_system_type_id] = system;
        }
        else
        {
            assert(true);
        }

        if (static_system_type_id + 1 > this->system_dependency_matrix.size())
        {
            this->system_dependency_matrix.resize(static_system_type_id + 1);
            for (int i = 0; i < this->system_dependency_matrix.size(); ++i)
            {
                this->system_dependency_matrix[i].resize(static_system_type_id + 1);
            }
        }

        this->system_work_order.push_back(system);

        return system;
    }

    template <typename System, class Dependency>
    void addSystemDependency(System target, Dependency dependency)
    {
        const U64 target_id = target->getStaticSystemTypeID();
        const U64 depend_id = dependency->getStaticSystemTypeID();

        if (this->system_dependency_matrix[target_id][depend_id] != true)
        {
            this->system_dependency_matrix[target_id][depend_id] = true;
            //LogInfo("added '%s' as dependency to '%s'", dependency->GetSystemTypeName(), target->GetSystemTypeName())
        }

        // this->UpdateSystemWorkOrder();
    }

    template <typename Target, class Dependency, class... Dependencies>
    void addSystemDependency(Target target, Dependency dependency, Dependencies&&... dependencies)
    {
        const U64 target_id = target->getStaticSystemTypeID();
        const U64 depend_id = dependency->getStaticSystemTypeID();

        if (this->system_dependency_matrix[target_id][depend_id] != true)
        {
            this->system_dependency_matrix[target_id][depend_id] = true;
            //LogInfo("added '%s' as dependency to '%s'", dependency->GetSystemTypeName(), target->GetSystemTypeName())
        }

        this->addSystemDependency(target, std::forward<Dependencies>(dependencies)...);
    }

    void updateSystemWorkOrder();

    template <typename T>
    [[nodiscard]] T* getSystem() const
    {
        auto it = this->system_registry.find(T::STATIC_SYSTEM_TYPE_ID);
        return it != this->system_registry.end() ? (T*)it->second : nullptr;
    }

    template <typename T>
    void enableSystem()
    {
        const SystemTypeId system_type_id = T::STATIC_SYSTEM_TYPE_ID;

        auto it = this->system_registry.find(system_type_id);
        if (it != this->system_registry.end())
        {
            if (it->second->isEnabled())
            {
                return;
            }

            it->second->enable();
        }
        else
        {
            //LogWarning("Trying to enable system [%d], but system is not registered yet.", system_type_id);
        }
    }

    template <typename T>
    void disableSystem()
    {
        const SystemTypeId system_type_id = T::STATIC_SYSTEM_TYPE_ID;

        auto it = this->system_registry.find(system_type_id);
        if (it != this->system_registry.end())
        {
            if (!it->second->isEnabled())
            {
                return;
            }

            it->second->disable();
        }
        else
        {
            //LogWarning("Trying to disable system [%d], but system is not registered yet.", system_type_id);
        }
    }

    template <typename T>
    void setSystemUpdateInterval(float update_interval_ms)
    {
        const SystemTypeId system_type_id = T::STATIC_SYSTEM_TYPE_ID;

        auto it = this->system_registry.find(system_type_id);
        if (it != this->system_registry.end())
        {
            it->second->setUpdateInterval(update_interval_ms);
        }
        else
        {
            //LogWarning("Trying to change system's [%d] update interval, but system is not registered yet.", system_type_id);
        }
    }

    template <typename T>
    void setSystemPriority(SystemPriority new_priority)
    {
        const SystemTypeId system_type_id = T::STATIC_SYSTEM_TYPE_ID;

        auto it = this->system_registry.find(system_type_id );
        if (it != this->system_registry.end())
        {
            SystemPriority old_priority = it->second->getPriority();

            if (old_priority == new_priority)
            {
                return;
            }

            it->second->setPriority(new_priority);

            // re-build system work order
            // this->UpdateSystemWorkOrder();
        }
        else
        {
            //LogWarning("Trying to change system's [%d] priority, but system is not registered yet.", system_type_id);
        }
    }


    using SystemWorkStateMask = Vector<bool>;

    [[nodiscard]] SystemWorkStateMask getSystemWorkState() const;

    void setSystemWorkState(SystemWorkStateMask mask);

    template <typename... ActiveSystems>
    SystemWorkStateMask generateActiveSystemWorkState(ActiveSystems&&... active_systems)
    {
        SystemWorkStateMask mask(this->system_work_order.size(), false);
        List<ISystem*> active_systems_list = { active_systems... };
        for (auto system : active_systems_list)
        {
            for (U32 i = 0; i < this->system_work_order.size(); ++i)
            {
                if (this->system_work_order[i]->getStaticSystemTypeId() == system->getStaticSystemTypeId())
                {
                    mask[i] = true;
                    break;
                }
            }
        }
        return mask;
    }

private:
    using SystemDependencyMatrix = Vector<Vector<bool>>;
    using SystemRegistry = UnorderedMap<U64, ISystem*>;
    using SystemAllocator = memory::allocator::LinearAllocator;
    using SystemWorkOrder = Vector<ISystem*>;

    SystemAllocator* system_allocator;
    SystemRegistry system_registry;
    SystemDependencyMatrix system_dependency_matrix;
    SystemWorkOrder system_work_order;
};

} // namespace gg;
