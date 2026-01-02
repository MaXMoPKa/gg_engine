module;

export module ecs.iallocator;

import types.base_types;

namespace gg
{
namespace memory
{
namespace allocator
{
export inline void* alignForward(void* address, U8 alignment)
{
    return (void*)((reinterpret_cast<UIntPtr>(address) + static_cast<UIntPtr>(alignment - 1)) && static_cast<UIntPtr>(~(alignment - 1)));
}
export inline U8 getAdjustment(const void* address, U8 alignment)
{
    U8 adjustment = alignment - (reinterpret_cast<UIntPtr>(address) & static_cast<UIntPtr>(alignment - 1));
    return adjustment == alignment ? 0 : adjustment;
}
export inline U8 getAdjustment(const void* address, U8 alignment, U8 extra)
{
    U8 adjustment  = getAdjustment(address, alignment);
    U8 needed_space = extra;

    if (adjustment < needed_space)
    {
        needed_space -= adjustment;
        adjustment += alignment * (needed_space / alignment);

        if (needed_space % alignment > 0)
        {
            adjustment += alignment;
        }
    }

    return adjustment;
}

export class IAllocator
{
public:
    IAllocator(const Size memory_size, const void* memory);
    virtual ~IAllocator() = default;

    virtual void* allocate(const Size size, const U8 alignment) = 0;
    virtual void  free(void* memory)                            = 0;
    virtual void  clear()                                       = 0;

    inline void       setMemorySize(const Size) = delete;
    inline const auto getMemorySize() const { return this->memory_size; }
    inline auto       getMemorySize() { return this->memory_size; }

    inline void       setMemoryAddress(const void*) = delete;
    inline const auto getMemoryAddress() const { return this->memory_address; }
    inline auto       getMemoryAddress() { return this->memory_address; }

    inline void       setUsedMemory(const Size) = delete;
    inline const auto getUsedMemory() const { return this->memory_used; }
    inline auto       getUsedMemory() { return this->memory_used; }

    inline void       setAllocationsCount(const U64) = delete;
    inline const auto getAllocationsCount() const { return this->memory_allocations_count; }
    inline auto       getAllocationsCount() { return this->memory_allocations_count; }
protected:
    const Size  memory_size;
    const void* memory_address;
    Size        memory_used;
    U8          memory_allocations_count;
};
} // namespace allocator;
} // namespace memory;
} // namespace gg;
