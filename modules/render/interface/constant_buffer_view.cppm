module;

#include <memory>

#include <d3d12.h>

export module render:constant_buffer_view;

import :descriptor_allocation;

namespace gg
{

class ConstantBuffer;
class Device;

export class ConstantBufferView
{
public:
    [[nodiscard]] inline std::shared_ptr<ConstantBuffer> getConstantBuffer() const
    {
        return this->constant_buffer;
    }

    [[nodiscard]] inline D3D12_CPU_DESCRIPTOR_HANDLE getDescriptorHandle()
    {
        return this->descriptor.getDescriptorHandle();
    }
protected:
    ConstantBufferView(Device& device, const std::shared_ptr<ConstantBuffer>& constant_buffer, size_t offset = 0);
    virtual ~ConstantBufferView() = default;
private:
    Device& device;
    std::shared_ptr<ConstantBuffer> constant_buffer;
    DescriptorAllocation descriptor;
};

} // namespace gg;
