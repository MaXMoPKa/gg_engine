module;

#include <cassert>

#include <d3dx12.h>

#include <d3d12.h>

#include <wrl.h>

module render;

import :index_buffer;
import :buffer;
import :device;

namespace gg
{

IndexBuffer::IndexBuffer(Device& device, size_t num_indices, DXGI_FORMAT index_format)
    : Buffer(device, CD3DX12_RESOURCE_DESC::Buffer(num_indices * (index_format == DXGI_FORMAT_R16_UINT ? 2 : 4)))
    , num_indices(num_indices)
    , index_format(index_format)
    , index_buffer_view{}
{
    assert(index_format == DXGI_FORMAT_R16_UINT || index_format == DXGI_FORMAT_R32_UINT);
    this->createIndexBufferView();
}

IndexBuffer::IndexBuffer(Device& device, Microsoft::WRL::ComPtr<ID3D12Resource> resource, size_t num_indices, DXGI_FORMAT index_format)
    : Buffer(device, resource)
    , num_indices(num_indices)
    , index_format(index_format)
    , index_buffer_view{}
{
    assert(index_format == DXGI_FORMAT_R16_UINT || index_format == DXGI_FORMAT_R32_UINT);
    this->createIndexBufferView();
}

void IndexBuffer::createIndexBufferView()
{
    UINT buffer_size = this->num_indices * (this->index_format == DXGI_FORMAT_R16_UINT ? 2 : 4);

    this->index_buffer_view.BufferLocation = this->d3d12_resource->GetGPUVirtualAddress();
    this->index_buffer_view.SizeInBytes = buffer_size;
    this->index_buffer_view.Format = this->index_format;
}

} // namespace gg;
