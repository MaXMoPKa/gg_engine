module;

#include <string>
#include <cstddef>
#include <vector>
#include <list>
#include <utility>

export module ecs:memory_manager;

import :stack_allocator;
import :common;

namespace gg
{
namespace memory
{

export class MemoryManager
{
    using StackAllocator = allocator::StackAllocator;
public:
    MemoryManager();
    ~MemoryManager();

    void* allocate(std::size_t memory_size, const std::string& user = "");
    void free(void* pointer_memory);
    void check_memory_leaks();
public:
    static constexpr std::size_t MEMORY_CAPACITY = ECS_GLOBAL_MEMORY_CAPACITY;

private:
    MemoryManager(const MemoryManager&) = delete;
    MemoryManager& operator=(MemoryManager&) = delete;

private:
    void* global_memory;
    StackAllocator* memory_allocator;
    std::vector<std::pair<const std::string, void*>> pending_memory;
    std::list<void*> freed_memory;
};

} // namespace memory;
} // namespace gg;
