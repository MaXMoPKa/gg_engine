module;

module ecs.system_manager;

import ecs.isystem;

namespace gg
{

SystemManager::~SystemManager()
{
    for (ISystem* i_system : this->system_work_order)
    {
        i_system->~ISystem();
        i_system = nullptr;
    }

    system_work_order.clear();
    system_registry.clear();

    free((void*)this->system_allocator->getMemoryAddress());
    delete this->system_allocator;
    this->system_allocator = nullptr;

    //LogInfo("Release SystemManager!");
}

} // namespace gg;
