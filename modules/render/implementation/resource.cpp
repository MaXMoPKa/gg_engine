module;

#include <string>
#include <memory>

#include <d3d12.h>

#include <wrl.h>

module render;

import :resource;

namespace gg
{

void Resource::setName(const std::wstring& name)
{
    this->resource_name = name;
    if(this->d3d12_resource && !this->resource_name.empty())
    {
        this->d3d12_resource->SetName(this->resource_name.c_str());
    }
}

bool Resource::checkFormatSupport(D3D12_FORMAT_SUPPORT1 format_support) const
{
    return (this->format_support.Support1 & format_support) != 0;
}

bool Resource::checkFormatSupport(D3D12_FORMAT_SUPPORT2 format_support) const
{
    return (this->format_support.Support2 & format_support) != 0;
}

Resource::Resource(Device& device, const D3D12_RESOURCE_DESC& resource_desc, const D3D12_CLEAR_VALUE* clear_value)
    : device{device}
{
    Microsoft::WRL::ComPtr<ID3D12Device2> d3d12_device = this->device.getD3D12Device();

    if(clear_value)
    {
        this->d3d12_clear_value = std::make_unique<D3D12_CLEAR_VALUE>(*clear_value);
    }

    D3D12_HEAP_PROPERTIES heap_properties = {};
    heap_properties.Type = D3D12_HEAP_TYPE_DEFAULT;
    heap_properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heap_properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    heap_properties.CreationNodeMask = 1;
    heap_properties.VisibleNodeMask = 1;
    throwIfFailed(d3d12_device->CreateCommittedResource(&heap_properties, D3D12_HEAP_FLAG_NONE, &resource_desc,
                                                       D3D12_RESOURCE_STATE_COMMON, this->d3d12_clear_value.get(), IID_PPV_ARGS(&this->d3d12_resource)));
    ResourceStateTracker::addGlobalResourceState(this->d3d12_resource.Get(), D3D12_RESOURCE_STATE_COMMON);

    this->checkFeatureSupport();
}

Resource::Resource(Device& device, Microsoft::WRL::ComPtr<ID3D12Resource> resource, const D3D12_CLEAR_VALUE* clear_value)
    : device{device}
    , d3d12_resource{resource}
{
    if(clear_value)
    {
        this->d3d12_clear_value = std::make_unique<D3D12_CLEAR_VALUE>(*clear_value);
    }
    this->checkFeatureSupport();
}

void Resource::checkFeatureSupport()
{
    Microsoft::WRL::ComPtr<ID3D12Device2> d3d12_device = this->device.getD3D12Device();

    D3D12_RESOURCE_DESC desc = this->d3d12_resource->GetDesc();
    this->format_support.Format = desc.Format;
    throwIfFailed(d3d12_device->CheckFeatureSupport(D3D12_FEATURE_FORMAT_SUPPORT, &this->format_support, sizeof(D3D12_FEATURE_DATA_FORMAT_SUPPORT)));
}
 
} // namespace gg;
