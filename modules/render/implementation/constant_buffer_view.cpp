module;

#include <memory>
#include <cassert>

#include <d3d12.h>

#include <wrl.h>

module render;

import :constant_buffer_view;
import :descriptor_allocation;
import :constant_buffer;
import :device;
import gg.render.helpers;

namespace gg
{

ConstantBufferView::ConstantBufferView(Device& device, const std::shared_ptr<ConstantBuffer>& constant_buffer, size_t offset)
    : device(device)
    , constant_buffer(constant_buffer)
{
    assert(this->constant_buffer);

    Microsoft::WRL::ComPtr<ID3D12Device2> d3d12_device = this->device.getD3D12Device();
    Microsoft::WRL::ComPtr<ID3D12Resource> d3d12_resource = this->constant_buffer->getD3D12Resource();

    D3D12_CONSTANT_BUFFER_VIEW_DESC cbv;
    cbv.BufferLocation = d3d12_resource->GetGPUVirtualAddress() + offset;
    cbv.SizeInBytes = math::alignUp(this->constant_buffer->getSizeInBytes(), D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);

    this->descriptor = this->device.allocateDescriptors(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    d3d12_device->CreateConstantBufferView(&cbv, this->descriptor.getDescriptorHandle());
}

} // namespace gg;
