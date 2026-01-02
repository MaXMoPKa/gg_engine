module;

#include <cassert>

module ecs.linear_allocator;

namespace gg
{
namespace memory
{
namespace allocator
{

LinearAllocator::LinearAllocator(const Size memory_size, const void* memory)
    : IAllocator(memory_size, memory)
{}

LinearAllocator::~LinearAllocator()
{
    this->clear();
}

void* LinearAllocator::allocate(const Size size, const U8 alignment)
{
    assert(size > 0 && "allocate calles with memSize = 0.");

    union
    {
        void* as_void_pointer;
        UIntPtr as_uintptr;
    };

    as_void_pointer = (void*)this->memory_address;
    as_uintptr += this->memory_used;

    U8 adjustment = getAdjustment(as_void_pointer, alignment);

    if (this->memory_used + size + adjustment > this->memory_size)
    {
        return nullptr;
    }

    as_uintptr += adjustment;

    this->memory_used += size + adjustment;
    this->memory_allocations_count++;

    return as_void_pointer;
}

void LinearAllocator::free(void* memory)
{
    assert(false && "Linear allocators do not support free operations. Use clear unstead.");
}

void LinearAllocator::clear()
{
    this->memory_used             = 0;
    this->memory_allocations_count = 0;
}

} // namespace allocator;
} // namespace memory;
} // namespace gg;
