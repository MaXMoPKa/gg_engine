module;

#include <d3d12.h>

#include <wrl.h>

module render;

import :buffer;
import :device;
import :resource;

namespace gg
{

Buffer::Buffer(Device& device, const D3D12_RESOURCE_DESC& desc)
    : Resource(device, desc)
{}

Buffer::Buffer(Device& device, Microsoft::WRL::ComPtr<ID3D12Resource> resource)
    : Resource(device, resource)
{}

} // namespace gg;
