module;

#include <cstddef>
#include <cstdint>

export module ecs:pool_allocator;

import :iallocator;

namespace gg
{
namespace memory
{
namespace allocator
{
class PoolAllocator : public IAllocator
{
public:
    PoolAllocator(const std::size_t memory_size,
                  const void*       memory,
                  const std::size_t object_size,
                  const uint8_t     object_alignment);
    virtual ~PoolAllocator();

    virtual void* allocate(const std::size_t size, const uint8_t alignment) override;
    virtual void  free(void* memory) override;
    virtual void  clear() override;

private:
    const std::size_t object_size;
    const uint8_t     object_alignment;
    void**            free_list;
};
} // namespace allocator;
} // namespace memory;
} // namespace gg;
