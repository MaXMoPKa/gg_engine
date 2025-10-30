module;

#include <d3d12.h>

#include <wrl.h>

export module render:byte_address_buffer;

import :buffer;
import :descriptor_allocation;

namespace gg
{

class Device;

export class ByteAddressBuffer : public Buffer
{
public:
    [[nodiscard]] inline size_t getBufferSize() const
    {
        return this->buffer_size;
    }
protected:
    ByteAddressBuffer(Device& device, const D3D12_RESOURCE_DESC& desc);
    ByteAddressBuffer(Device& device, Microsoft::WRL::ComPtr<ID3D12Resource> resource);
    virtual ~ByteAddressBuffer() = default;
private:
    size_t buffer_size;
};

} // namespace gg;
