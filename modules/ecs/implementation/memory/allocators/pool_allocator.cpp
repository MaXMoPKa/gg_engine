module;

#include <cassert>
#include <cmath>

module ecs.pool_allocator;

namespace gg
{
namespace memory
{
namespace allocator
{
PoolAllocator::PoolAllocator(const Size  memory_size,
                             const void* memory,
                             const Size  object_size,
                             const U8    object_alignment)
    : IAllocator(memory_size, memory)
    , object_size(object_size)
    , object_alignment(object_alignment)
{
    this->clear();
}

PoolAllocator::~PoolAllocator()
{
    this->free_list = nullptr;
}

void* PoolAllocator::allocate(const Size size, const U8 alignment)
{
    assert(size > 0 && "allocate called with memorySize = 0.");
    assert(size == this->object_size && alignment == this->object_alignment);

    if (this->free_list == nullptr)
    {
        return nullptr;
    }

    void* pointer = this->free_list;

    this->free_list = (void**)(*this->free_list);

    this->memory_used += this->object_size;
    this->memory_allocations_count++;

    return pointer;
}

void PoolAllocator::free(void* memory)
{
    *((void**)memory) = this->free_list;
    this->free_list    = (void**)memory;

    this->memory_used -= this->object_size;
    this->memory_allocations_count--;
}

void PoolAllocator::clear()
{
    U8 adjustment = getAdjustment(this->memory_address, this->object_alignment);
    Size number_objects = (Size)std::floor((this->memory_size - adjustment) / this->object_size);

    union
    {
        void* as_void_pointer;
        UIntPtr as_uintptr;
    };

    as_void_pointer = (void*)this->memory_address;

    as_uintptr += adjustment;

    this->free_list = (void**)as_void_pointer;

    void** pointer = this->free_list;

    for (U32 i = 0; i < (number_objects - 1); ++i)
    {
        *pointer = (void*)((UIntPtr)pointer + this->object_size);
        pointer  = (void**)*pointer;
    }

    *pointer = nullptr;
}

} // namespace allocator;
} // namespace memory;
} // namespace gg;
