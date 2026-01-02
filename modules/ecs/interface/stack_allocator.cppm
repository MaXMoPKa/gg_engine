module;

export module ecs.stack_allocator;

import ecs.iallocator;

import types.base_types;

namespace gg
{
namespace memory
{
namespace allocator
{
export class StackAllocator : public IAllocator
{
public:
    StackAllocator(const Size memory_size, const void* memory);
    virtual ~StackAllocator();

    virtual void* allocate(const Size size, const U8 alognment) override;
    virtual void free(void* memory) override;
    virtual void clear() override;

private:
    struct AllocatorMetaInfo
    {
        U8 adjusment;
    };
};
} // namespace allocator;
} // namespace memory;
} // namespace gg;
