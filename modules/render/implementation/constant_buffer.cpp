module;

#include <d3d12.h>

#include <wrl.h>

module render;

import :constant_buffer;
import :device;

namespace gg
{

ConstantBuffer::ConstantBuffer(Device& device, Microsoft::WRL::ComPtr<ID3D12Resource> resource)
    : Buffer(device, resource)
{}

ConstantBuffer::~ConstantBuffer() {}

} // namespace gg;
