module;

#include <utility>

export module ecs.memory_manager;

import ecs.stack_allocator;
import ecs.common;

import types.base_types;
import types.string;
import types.vector;
import types.list;
import types.pair;

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

    void* allocate(Size memory_size, const String& user = "");
    void free(void* pointer_memory);
    void check_memory_leaks();
public:
    static constexpr Size MEMORY_CAPACITY = ECS_GLOBAL_MEMORY_CAPACITY;

private:
    MemoryManager(const MemoryManager&) = delete;
    MemoryManager& operator=(MemoryManager&) = delete;

private:
    void* global_memory;
    StackAllocator* memory_allocator;
    Vector<Pair<const String, void*>> pending_memory;
    List<void*> freed_memory;
};

} // namespace memory;
} // namespace gg;
