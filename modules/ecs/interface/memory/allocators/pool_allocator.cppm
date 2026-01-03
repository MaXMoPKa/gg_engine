module;

export module ecs.pool_allocator;

import ecs.iallocator;

import types.base_types;

namespace gg
{
namespace memory
{
namespace allocator
{
export class PoolAllocator : public IAllocator
{
public:
    PoolAllocator(const Size memory_size,
                  const void*       memory,
                  const Size object_size,
                  const U8     object_alignment);
    virtual ~PoolAllocator();

    virtual void* allocate(const Size size, const U8 alignment) override;
    virtual void  free(void* memory) override;
    virtual void  clear() override;

private:
    const Size object_size;
    const U8   object_alignment;
    void**     free_list;
};
} // namespace allocator;
} // namespace memory;
} // namespace gg;
