module;

export module ecs.entity;

import ecs.ientity;
import ecs.family_type_id;
import ecs.common;
import ecs.component_manager;

namespace gg
{
    
export template<typename T>
class Entity : public IEntity
{
public:
    static const EntityTypeId STATIC_ENTITY_TYPE_ID;

    Entity() = default;
    Entity(const EntityId& entity_id, ComponentManager* component_manager)
        : IEntity(entity_id, component_manager)
    {}

    virtual ~Entity() = default;

    virtual const EntityTypeId getStaticEntityTypeId() const override
    {
        return STATIC_ENTITY_TYPE_ID;
    }
};

template<typename T>
const EntityTypeId Entity<T>::STATIC_ENTITY_TYPE_ID = util::FamilyTypeId<IEntity>::get<T>();

} // namespace gg;
