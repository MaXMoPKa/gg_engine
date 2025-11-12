module;

module ecs;

import :global_memory_user;

namespace gg
{
namespace memory
{
GlobalMemoryUser::GlobalMemoryUser()
{}

void GlobalMemoryUser::free(void* memory)
{
    //ecs_memory_manager->free(memory);
}
} // namespace memory;
} // namespace gg;
