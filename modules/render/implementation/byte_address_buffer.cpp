module;

#include <d3d12.h>

#include <wrl.h>

module render;

import :byte_address_buffer;
import :buffer;
import :descriptor_allocation;
import :device;

namespace gg
{
ByteAddressBuffer::ByteAddressBuffer(Device& device, const D3D12_RESOURCE_DESC& desc)
    : Buffer(device, desc)
{}

ByteAddressBuffer::ByteAddressBuffer(Device& device, Microsoft::WRL::ComPtr<ID3D12Resource> resource)
    : Buffer(device, resource)
{}
} // namespace gg;
