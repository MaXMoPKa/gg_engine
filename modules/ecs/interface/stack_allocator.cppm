module;

#include <cstddef>
#include <cstdint>

export module ecs:stack_allocator;

import :iallocator;

namespace gg
{
namespace memory
{
namespace allocator
{
export class StackAllocator : public IAllocator
{
public:
    StackAllocator(const std::size_t memory_size, const void* memory);
    virtual ~StackAllocator();

    virtual void* allocate(const std::size_t size, const uint8_t alognment) override;
    virtual void free(void* memory) override;
    virtual void clear() override;

private:
    struct AllocatorMetaInfo
    {
        uint8_t adjusment;
    };
};
} // namespace allocator;
} // namespace memory;
} // namespace gg;
