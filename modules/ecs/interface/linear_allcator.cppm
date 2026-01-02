module;

export module ecs.linear_allocator;

import ecs.iallocator;

import types.base_types;

namespace gg
{
namespace memory
{
namespace allocator
{

export class LinearAllocator : public IAllocator
{
public:
    LinearAllocator(const Size memory_size, const void* memory);
    virtual ~LinearAllocator();

    virtual void* allocate(const Size size, const U8 alignment) override;
    virtual void free(void* memory) override;
    virtual void clear() override;
};

} // namespace allocator;
} // namespace memory;
} // namespace gg;
