module;

#include <memory>
#include <queue>
#include <map>
#include <mutex>
#include <cstdint>

#include <wrl.h>

#include <d3d12.h>

export module render:descriptor_allocator_page;

namespace gg
{

class DescriptorAllocation;
class Device;

export class DescriptorAllocatorPage : public std::enable_shared_from_this<DescriptorAllocatorPage>
{
public:
    [[nodiscard]] D3D12_DESCRIPTOR_HEAP_TYPE getHeapType() const;

    [[nodiscard]] bool hasSpace(uint32_t num_descriptors) const;

    [[nodiscard]] uint32_t numFreeHandles() const;

    [[nodiscard]] DescriptorAllocation allocate(uint32_t num_descriptors);

    void free(DescriptorAllocation&& descriptor);

    void releaseStaleDescriptors();
protected:
    DescriptorAllocatorPage(Device& device, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t num_descriptors);
    virtual ~DescriptorAllocatorPage() = default;

    uint32_t computeOffset(D3D12_CPU_DESCRIPTOR_HANDLE handle);

    void addNewBlock(uint32_t offset, uint32_t num_descriptors);

    void freeBlock(uint32_t offset, uint32_t num_descriptors);
private:
    using OffsetType = uint32_t;
    using SizeType = uint32_t;

    struct FreeBlockInfo;
    using FreeListByOffset = std::map<OffsetType, FreeBlockInfo>;
    using FreeListBySize = std::multimap<SizeType, FreeListByOffset::iterator>;

    struct FreeBlockInfo
    {
        FreeBlockInfo(SizeType size)
            : size{size}
        {}

        SizeType size;
        FreeListBySize::iterator free_list_by_size_iterator;
    };

    struct StaleDescriptorInfo
    {
        StaleDescriptorInfo(OffsetType offset, SizeType size)
            : offset{offset}
            , size{size}
        {}

        OffsetType offset;
        SizeType size;
    };

    Device& device;

    using StaleDescriptorQueue = std::queue<StaleDescriptorInfo>;

    FreeListByOffset free_list_by_offset;
    FreeListBySize free_list_by_size;
    StaleDescriptorQueue stale_descriptors;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> d3d12_descriptor_heap;
    D3D12_DESCRIPTOR_HEAP_TYPE d3d12_heap_type;
    D3D12_CPU_DESCRIPTOR_HANDLE d3d12_base_descriptor;
    uint32_t descriptor_handle_increment_size;
    uint32_t num_descriptors_in_heap;
    uint32_t num_free_handles;

    std::mutex allocation_mutex;
};

export class MakeDescriptorAllocatorPage : public DescriptorAllocatorPage
{
public:
    MakeDescriptorAllocatorPage(Device& device, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t num_descriptors);
    ~MakeDescriptorAllocatorPage() override;
};

} // namespace gg;
