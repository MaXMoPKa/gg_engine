module;

#include <utility>

export module ecs.ientity;

import ecs.common;
import ecs.component_manager;

import types.base_types;

namespace gg
{

export class IEntity
{
public:
    IEntity();
    IEntity(const EntityId& entity_id, ComponentManager* component_manager);
    virtual ~IEntity() = default;

    template<typename T>
    T* getComponent() const
    {
        return this->component_manager_instance->getComponent<T>(this->entity_id);
    }

    template<typename T, class... P>
    T* addComponent(P&&... params)
    {
        return this->component_manager_instance->addComponent<T>(this->entity_id, std::forward<P>(params)...);
    }

    template<typename T>
    void removeComponent()
    {
        this->component_manager_instance->removeComponent<T>(this->entity_id);
    }

    virtual void onEnable(){}
    virtual void OnDisable(){}

    [[nodiscard]] inline Bool operator==(const IEntity& rhs) const
    {
        return this->entity_id == rhs.entity_id;
    }
    [[nodiscard]] inline Bool operator!=(const IEntity& rhs) const
    {
        return this->entity_id != rhs.entity_id;
    }
    [[nodiscard]] inline Bool operator==(const IEntity* rhs) const
    {
        return this->entity_id == rhs->entity_id;
    }
    [[nodiscard]] inline Bool operator!=(const IEntity* rhs) const
    {
        return this->entity_id != rhs->entity_id;
    }

    virtual const EntityTypeId getStaticEntityTypeId() const = 0;

    [[nodiscard]] inline const EntityId getEntityId() const
    {
        return this->entity_id;
    }

    void setActive(Bool active);

    inline Bool isActive() const
    {
        return this->entity_id;
    }

protected:
    EntityId entity_id;
    Bool is_active;
    ComponentManager* component_manager_instance;
};

} // namespace gg;
