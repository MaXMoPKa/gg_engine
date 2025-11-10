module;

export module ecs:component;

import :icomponent;
import :family_type_id;

namespace gg
{

export template<typename T>
class Component : public IComponent
{
public:
    static const ComponentTypeId STATIC_COMPONENT_TYPE_ID;

    Component() = default;
    virtual ~Component() = default;

    [[nodiscard]] inline ComponentTypeId getStaticComponentTypeId() const
    {
        return STATIC_COMPONENT_TYPE_ID;
    }
};

template<typename T>
const ComponentTypeId Component<T>::STATIC_COMPONENT_TYPE_ID = util::FamilyTypeId<IComponent>::get<T>();

} // namespace gg;
