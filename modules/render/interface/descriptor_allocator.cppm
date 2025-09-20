module;

#include <vector>
#include <memory>
#include <set>
#include <mutex>

#include <d3d12.h>

export module render:descriptor_allocator;

import :descriptor_allocation;

namespace gg
{

class DescriptorAllocatorPage;
class Device;

export class DescriptorAllocator
{
public:
    DescriptorAllocation allocate(uint32_t num_descriptors = 1);

    void releaseStaleDescriptors();

protected:
    friend class std::default_delete<DescriptorAllocator>;

    DescriptorAllocator(Device& device, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t num_descriptors_per_heap = 256);
    virtual ~DescriptorAllocator();

private:
    std::shared_ptr<DescriptorAllocatorPage> createAllocatorPage();

    Device& device;
    D3D12_DESCRIPTOR_HEAP_TYPE heap_type;
    uint32_t num_descriptors_per_heap;

    using DescriptorHeapPool = std::vector<std::shared_ptr<DescriptorAllocatorPage>>;

    DescriptorHeapPool heap_pool;

    std::set<size_t> available_heaps;

    std::mutex allocation_mutex;
};

export class MakeDescriptorAllocator : public DescriptorAllocator
{
public:
    MakeDescriptorAllocator(Device& device, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t num_descriptors_per_heap = 256);
    ~MakeDescriptorAllocator() override;
};

} // namespace gg;
