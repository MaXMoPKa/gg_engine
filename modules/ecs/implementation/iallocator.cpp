module;

#include <cstddef>

module ecs;

import :iallocator;

namespace gg
{
namespace memory
{
namespace allocator
{
IAllocator::IAllocator(const std::size_t memory_size, const void* memory)
    : memory_size{memory_size}
    , memory_address{memory}
    , memory_used{0}
    , memory_allocations_count{0}
{}
} // namespace allocator;
} // namespace memory;
} // namespace gg;
