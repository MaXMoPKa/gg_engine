module;

#include <cstddef>
#include <cstdint>
#include <cassert>

module ecs;

import :linear_allocator;

namespace gg
{
namespace memory
{
namespace allocator
{

LinearAllocator::LinearAllocator(const std::size_t memory_size, const void* memory)
    : IAllocator(memory_size, memory)
{}

LinearAllocator::~LinearAllocator()
{
    this->clear();
}

void* LinearAllocator::allocate(const std::size_t size, const uint8_t alignment)
{
    assert(size > 0 && "allocate calles with memSize = 0.");

    union
    {
        void* as_void_pointer;
        uintptr_t  as_uintptr;
    };

    as_void_pointer = (void*)this->memory_address;
    as_uintptr += this->memory_used;

    uint8_t adjustment = getAdjustment(as_void_pointer, alignment);

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
