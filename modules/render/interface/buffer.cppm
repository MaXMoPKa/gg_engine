module;

#include <d3d12.h>

#include <wrl.h>

export module render:buffer;

import :resource;

namespace gg
{

class Device;

export class Buffer : public Resource
{
protected:
    Buffer(Device& device, const D3D12_RESOURCE_DESC& desc);
    Buffer(Device& device, Microsoft::WRL::ComPtr<ID3D12Resource> resource);
};

} // namespace gg;
