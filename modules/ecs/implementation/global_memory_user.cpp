module;

module ecs.global_memory_user;

namespace gg
{
namespace memory
{
GlobalMemoryUser::GlobalMemoryUser()
{
    this->memory_manager = new MemoryManager();
}

const void* GlobalMemoryUser::allocate(Size mem_size, const String& user)
{
    return this->memory_manager->allocate(mem_size, user);
}
    
void GlobalMemoryUser::free(void* memory)
{
    this->memory_manager->free(memory);
}
} // namespace memory;
} // namespace gg;
