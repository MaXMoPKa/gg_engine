module;

#include <memory>

#include <d3d12.h>

export module render:descriptor_allocation;

namespace gg
{

class DescriptorAllocatorPage;

export class DescriptorAllocation
{
public:
    DescriptorAllocation();

    DescriptorAllocation(D3D12_CPU_DESCRIPTOR_HANDLE descriptor, uint32_t num_handles, uint32_t descriptor_size, std::shared_ptr<DescriptorAllocatorPage> page);

    ~DescriptorAllocation();

    DescriptorAllocation(const DescriptorAllocation&) = delete;
    DescriptorAllocation& operator=(const DescriptorAllocation&) = delete;

    DescriptorAllocation(DescriptorAllocation&& other) noexcept;
    DescriptorAllocation& operator=(DescriptorAllocation&& other) noexcept;

    [[nodiscard]] bool is_null() const;
    [[nodiscard]] inline bool is_valid() const
    {
        return !this->is_null();
    }

    [[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE getDescriptorHandle(uint32_t offset = 0) const;

    [[nodiscard]] uint32_t getNumHandles() const;

    [[nodiscard]] std::shared_ptr<DescriptorAllocatorPage> getDescriptorAllocatorPage() const;
private:
    void free();

private:
    D3D12_CPU_DESCRIPTOR_HANDLE d3d12_descriptor;
    uint32_t num_handles;
    uint32_t descriptor_size;

    std::shared_ptr<DescriptorAllocatorPage> page;
};

} // namespace gg;
