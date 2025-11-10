module;

#include <cstdint>
#include <cstddef>

export module ecs:iallocator;

namespace gg
{
namespace memory
{
namespace allocator
{
export inline void* alignForward(void* address, uint8_t alignment)
{
    return (void*)((reinterpret_cast<uintptr_t>(address) + static_cast<uintptr_t>(alignment - 1)) && static_cast<uintptr_t>(~(alignment - 1)));
}
export inline uint8_t getAdjustment(const void* address, uint8_t alignment)
{
    uint8_t adjustment = alignment - (reinterpret_cast<uintptr_t>(address) & static_cast<uintptr_t>(alignment - 1));
    return adjustment == alignment ? 0 : adjustment;
}
export inline uint8_t getAdjustment(const void* address, uint8_t alignment, uint8_t extra)
{
    uint8_t adjustment  = getAdjustment(address, alignment);
    uint8_t needed_space = extra;

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
    IAllocator(const std::size_t memory_size, const void* memory);
    virtual ~IAllocator() = default;

    virtual void* allocate(const std::size_t size, const uint8_t alignment) = 0;
    virtual void  free(void* memory)                                   = 0;
    virtual void  clear()                                              = 0;

    inline void       setMemorySize(const std::size_t) = delete;
    inline const auto getMemorySize() const { return this->memory_size; }
    inline auto       getMemorySize() { return this->memory_size; }

    inline void       setMemoryAddress(const void*) = delete;
    inline const auto getMemoryAddress() const { return this->memory_address; }
    inline auto       getMemoryAddress() { return this->memory_address; }

    inline void       setUsedMemory(const std::size_t) = delete;
    inline const auto getUsedMemory() const { return this->memory_used; }
    inline auto       getUsedMemory() { return this->memory_used; }

    inline void       setAllocationsCount(const uint64_t) = delete;
    inline const auto getAllocationsCount() const { return this->memory_allocations_count; }
    inline auto       getAllocationsCount() { return this->memory_allocations_count; }
protected:
    const std::size_t memory_size;
    const void*       memory_address;
    std::size_t       memory_used;
    uint64_t          memory_allocations_count;
};
} // namespace allocator;
} // namespace memory;
} // namespace gg;
