module;

module ecs.component_manager;

import ecs.family_type_id;

namespace gg
{

ComponentManager::ComponentManager()
{
    const Size num_components{ util::FamilyTypeId<IComponent>::get() };

    this->entity_component_map.resize(ENITY_LUT_GROW);
    for (auto i = 0; i < ENITY_LUT_GROW; ++i)
    {
        this->entity_component_map[i].resize(num_components, INVALID_COMPONENT_ID);
    }
}

ComponentManager::~ComponentManager()
{
    for (auto cc : this->component_container_registry)
    {
        delete cc.second;
        cc.second = nullptr;
    }
}

} // namespace gg;
