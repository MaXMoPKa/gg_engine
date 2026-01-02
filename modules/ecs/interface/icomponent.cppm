module;

export module ecs.icomponent;

import ecs.common;

import types.base_types;

namespace gg
{
    
export class IComponent
{
public:
    IComponent()
        : owner(INVALID_ENTITY_ID)
        , enabled(true)
    {}
    virtual ~IComponent() = default;

    [[nodiscard]] inline const Bool operator==(const IComponent& other) const
    {
        return this->hash_value == other.hash_value;
    }
    [[nodiscard]] inline const Bool operator!=(const IComponent& other) const
    {
        return this->hash_value != other.hash_value;
    }

    [[nodiscard]] inline const ComponentId getHash() const
    {
        return this->hash_value;
    }
    void inline setHash(ComponentId new_hash_value)
    {
        this->hash_value = new_hash_value;
    }
        
    [[nodiscard]] inline const ComponentId getComponentId() const
    {
        return this->component_id;
    }

    [[nodiscard]] inline const EntityId getOwner() const
    {
        return this->owner;
    }
    void setOwner(EntityId new_owner)
    {
        this->owner = new_owner;
    }
        
    inline void setIsActive(Bool state)
    {
        this->enabled = state;
    }

    [[nodiscard]] inline Bool getIsActive() const
    {
        return this->enabled;
    }

protected:
    ComponentId hash_value;
    ComponentId component_id;
    EntityId owner;
    Bool enabled;
};

} // namespace gg;
