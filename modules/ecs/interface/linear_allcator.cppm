module;

#include <cstdint>
#include <cstddef>

export module ecs:linear_allocator;

import :iallocator;

namespace gg
{
namespace memory
{
namespace allocator
{

export class LinearAllocator : public IAllocator
{
public:
    LinearAllocator(const std::size_t memory_size, const void* memory);
    virtual ~LinearAllocator();

    virtual void* allocate(const std::size_t size, const uint8_t alignment) override;
    virtual void free(void* memory) override;
    virtual void clear() override;
};

} // namespace allocator;
} // namespace memory;
} // namespace gg;
