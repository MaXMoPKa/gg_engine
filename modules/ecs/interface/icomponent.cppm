module;

export module ecs:icomponent;

import :common;

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

    [[nodiscard]] inline const bool operator==(const IComponent& other) const
    {
        return this->hash_value == other.hash_value;
    }
    [[nodiscard]] inline const bool operator!=(const IComponent& other) const
    {
        return this->hash_value != other.hash_value;
    }

    [[nodiscard]] inline const ComponentId getComponentId() const
    {
        return this->component_id;
    }

    [[nodiscard]] inline const EntityId getOwner() const
    {
        return this->owner;
    }

    inline void setIsActive(bool state)
    {
        this->enabled = state;
    }

    [[nodiscard]] inline bool getIsActive() const
    {
        return this->enabled;
    }

protected:
    ComponentId hash_value;
    ComponentId component_id;
    EntityId owner;
    bool enabled;

private:
    friend class ComponentManager;
};

} // namespace gg;
