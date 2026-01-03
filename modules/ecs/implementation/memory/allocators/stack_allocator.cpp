module;

#include <cassert>

module ecs.stack_allocator;

namespace gg
{
namespace memory
{
namespace allocator
{

StackAllocator::StackAllocator(const Size memory_size, const void* memory)
    : IAllocator(memory_size, memory)
{}

StackAllocator::~StackAllocator()
{
    this->clear();
}

void* StackAllocator::allocate(const Size size, const U8 alignment)
{
    assert(size > 0 && "allocate called with memory_size == 0.");

    union
    {
        void* as_void_pointer;
        UIntPtr as_uintptr;
        AllocatorMetaInfo* as_meta_info;
    };

    as_void_pointer = (void*)this->memory_address;

    as_uintptr += this->memory_used;

    U8 adjustment = getAdjustment(as_void_pointer, alignment, sizeof(AllocatorMetaInfo));

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
        UIntPtr as_uintptr;
        AllocatorMetaInfo* as_meta_info;
    };

    as_void_pointer = memory;
    as_uintptr -= sizeof(AllocatorMetaInfo);

    this->memory_used -= ((UIntPtr)this->memory_address + this->memory_used) - ((UIntPtr)memory + as_meta_info->adjusment);
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
