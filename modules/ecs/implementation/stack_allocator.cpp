module;

#include <cassert>
#include <cstdint>
#include <cstddef>

module ecs;

import :stack_allocator;

namespace gg
{
namespace memory
{
namespace allocator
{

StackAllocator::StackAllocator(const std::size_t memory_size, const void* memory)
    : IAllocator(memory_size, memory)
{}

StackAllocator::~StackAllocator()
{
    this->clear();
}

void* StackAllocator::allocate(const std::size_t size, const uint8_t alignment)
{
    assert(size > 0 && "allocate called with memory_size == 0.");

    union
    {
        void* as_void_pointer;
        uintptr_t as_uintptr;
        AllocatorMetaInfo* as_meta_info;
    };

    as_void_pointer = (void*)this->memory_address;

    as_uintptr += this->memory_used;

    uint8_t adjustment = getAdjustment(as_void_pointer, alignment, sizeof(AllocatorMetaInfo));

    if(this->memory_used + size + adjustment > this->memory_size)
    {
        return nullptr;
    }

    as_meta_info->adjusment = adjustment;

    as_uintptr += adjustment;

    this->memory_used += size + adjustment;
    this->memory_allocations_count++;

    return as_void_pointer;
}

void StackAllocator::free(void* memory)
{
    union
    {
        void* as_void_pointer;
        uintptr_t as_uintptr;
        AllocatorMetaInfo* as_meta_info;
    };

    as_void_pointer = memory;
    as_uintptr -= sizeof(AllocatorMetaInfo);

    this->memory_used -= ((uintptr_t)this->memory_address + this->memory_used) - ((uintptr_t)memory + as_meta_info->adjusment);
    this->memory_allocations_count--;
}

void StackAllocator::clear()
{
    this->memory_used = 0;
    this->memory_allocations_count = 0;
}

} // namespace allocator;
} // namespace memory;
} // namespace gg;
