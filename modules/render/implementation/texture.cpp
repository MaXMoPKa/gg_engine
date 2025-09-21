module;

#define NOMINMAX

#include <cstdint>
#include <utility>

#include <DirectXTex.h>
#include <d3d12.h>

#include <wrl.h>

module render;

import :texture;
import :device;

namespace gg
{

bool Texture::isUAVCompatibleFormat(DXGI_FORMAT format)
{
    switch(format)
    {
        case DXGI_FORMAT_R32G32B32A32_FLOAT:
        case DXGI_FORMAT_R32G32B32A32_UINT:
        case DXGI_FORMAT_R32G32B32A32_SINT:
        case DXGI_FORMAT_R16G16B16A16_FLOAT:
        case DXGI_FORMAT_R16G16B16A16_UINT:
        case DXGI_FORMAT_R16G16B16A16_SINT:
        case DXGI_FORMAT_R8G8B8A8_UNORM:
        case DXGI_FORMAT_R8G8B8A8_UINT:
        case DXGI_FORMAT_R8G8B8A8_SINT:
        case DXGI_FORMAT_R32_FLOAT:
        case DXGI_FORMAT_R32_UINT:
        case DXGI_FORMAT_R32_SINT:
        case DXGI_FORMAT_R16_FLOAT:
        case DXGI_FORMAT_R16_UINT:
        case DXGI_FORMAT_R16_SINT:
        case DXGI_FORMAT_R8_UNORM:
        case DXGI_FORMAT_R8_UINT:
        case DXGI_FORMAT_R8_SINT:
        {
            return true;
        }
        default:
        {
            return false;
        }
    }
}

bool Texture::isSRGBFormat(DXGI_FORMAT format)
{
    switch(format)
    {
        case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
        case DXGI_FORMAT_BC1_UNORM_SRGB:
        case DXGI_FORMAT_BC2_UNORM_SRGB:
        case DXGI_FORMAT_BC3_UNORM_SRGB:
        case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
        case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
        case DXGI_FORMAT_BC7_UNORM_SRGB:
        {
            return true;
        }
        default:
        {
            return false;
        }
    }
}

bool Texture::isBGRFormat(DXGI_FORMAT format)
{
    switch(format)
    {
        case DXGI_FORMAT_B8G8R8A8_UNORM:
        case DXGI_FORMAT_B8G8R8X8_UNORM:
        case DXGI_FORMAT_B8G8R8A8_TYPELESS:
        case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
        case DXGI_FORMAT_B8G8R8X8_TYPELESS:
        case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
        {
            return true;
        }
        default:
        {
            return false;
        }
    }
}

bool Texture::isDepthFormat(DXGI_FORMAT format)
{
    switch(format)
    {
        case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
        case DXGI_FORMAT_D32_FLOAT:
        case DXGI_FORMAT_D24_UNORM_S8_UINT:
        case DXGI_FORMAT_D16_UNORM:
        {
            return true;
        }
        default:
        {
            return false;
        }
    }
}

DXGI_FORMAT Texture::getTypelessFormat(DXGI_FORMAT format)
{
    DXGI_FORMAT typeless_format = format;

    switch(format)
    {
        case DXGI_FORMAT_R32G32B32A32_FLOAT:
        case DXGI_FORMAT_R32G32B32A32_UINT:
        case DXGI_FORMAT_R32G32B32A32_SINT:
        {
            typeless_format = DXGI_FORMAT_R32G32B32A32_TYPELESS;
            break;
        }
        case DXGI_FORMAT_R32G32B32_FLOAT:
        case DXGI_FORMAT_R32G32B32_UINT:
        case DXGI_FORMAT_R32G32B32_SINT:
        {
            typeless_format = DXGI_FORMAT_R32G32B32_TYPELESS;
            break;
        }
        case DXGI_FORMAT_R16G16B16A16_FLOAT:
        case DXGI_FORMAT_R16G16B16A16_UNORM:
        case DXGI_FORMAT_R16G16B16A16_UINT:
        case DXGI_FORMAT_R16G16B16A16_SNORM:
        case DXGI_FORMAT_R16G16B16A16_SINT:
        {
            typeless_format = DXGI_FORMAT_R16G16B16A16_TYPELESS;
            break;
        }
        case DXGI_FORMAT_R32G32_FLOAT:
        case DXGI_FORMAT_R32G32_UINT:
        case DXGI_FORMAT_R32G32_SINT:
        {
            typeless_format = DXGI_FORMAT_R32G32_TYPELESS;
            break;
        }
        case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
        {
            typeless_format = DXGI_FORMAT_R32G8X24_TYPELESS;
            break;
        }
        case DXGI_FORMAT_R10G10B10A2_UNORM:
        case DXGI_FORMAT_R10G10B10A2_UINT:
        {
            typeless_format = DXGI_FORMAT_R10G10B10A2_TYPELESS;
            break;
        }
        case DXGI_FORMAT_R8G8B8A8_UNORM:
        case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
        case DXGI_FORMAT_R8G8B8A8_UINT:
        case DXGI_FORMAT_R8G8B8A8_SNORM:
        case DXGI_FORMAT_R8G8B8A8_SINT:
        {
            typeless_format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
            break;
        }
        case DXGI_FORMAT_R16G16_FLOAT:
        case DXGI_FORMAT_R16G16_UNORM:
        case DXGI_FORMAT_R16G16_UINT:
        case DXGI_FORMAT_R16G16_SNORM:
        case DXGI_FORMAT_R16G16_SINT:
        {
            typeless_format = DXGI_FORMAT_R16G16_TYPELESS;
            break;
        }
        case DXGI_FORMAT_D32_FLOAT:
        case DXGI_FORMAT_R32_FLOAT:
        case DXGI_FORMAT_R32_UINT:
        case DXGI_FORMAT_R32_SINT:
        {
            typeless_format = DXGI_FORMAT_R32_TYPELESS;
            break;
        }
        case DXGI_FORMAT_R8G8_UNORM:
        case DXGI_FORMAT_R8G8_UINT:
        case DXGI_FORMAT_R8G8_SNORM:
        case DXGI_FORMAT_R8G8_SINT:
        {
            typeless_format = DXGI_FORMAT_R8G8_TYPELESS;
            break;
        }
        case DXGI_FORMAT_R16_FLOAT:
        case DXGI_FORMAT_D16_UNORM:
        case DXGI_FORMAT_R16_UNORM:
        case DXGI_FORMAT_R16_UINT:
        case DXGI_FORMAT_R16_SNORM:
        case DXGI_FORMAT_R16_SINT:
        {
            typeless_format = DXGI_FORMAT_R16_TYPELESS;
            break;
        }
        case DXGI_FORMAT_R8_UNORM:
        case DXGI_FORMAT_R8_UINT:
        case DXGI_FORMAT_R8_SNORM:
        case DXGI_FORMAT_R8_SINT:
        {
            typeless_format = DXGI_FORMAT_R8_TYPELESS;
            break;
        }
        case DXGI_FORMAT_BC1_UNORM:
        case DXGI_FORMAT_BC1_UNORM_SRGB:
        {
            typeless_format = DXGI_FORMAT_BC1_TYPELESS;
            break;
        }
        case DXGI_FORMAT_BC2_UNORM:
        case DXGI_FORMAT_BC2_UNORM_SRGB:
        {
            typeless_format = DXGI_FORMAT_BC2_TYPELESS;
            break;
        }
        case DXGI_FORMAT_BC3_UNORM:
        case DXGI_FORMAT_BC3_UNORM_SRGB:
        {
            typeless_format = DXGI_FORMAT_BC3_TYPELESS;
            break;
        }
        case DXGI_FORMAT_BC4_UNORM:
        case DXGI_FORMAT_BC4_SNORM:
        {
            typeless_format = DXGI_FORMAT_BC4_TYPELESS;
            break;
        }
        case DXGI_FORMAT_BC5_UNORM:
        case DXGI_FORMAT_BC5_SNORM:
        {
            typeless_format = DXGI_FORMAT_BC5_TYPELESS;
            break;
        }
        case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
        {
            typeless_format = DXGI_FORMAT_B8G8R8A8_TYPELESS;
            break;
        }
        case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
        {
            typeless_format = DXGI_FORMAT_B8G8R8X8_TYPELESS;
            break;
        }
        case DXGI_FORMAT_BC6H_UF16:
        case DXGI_FORMAT_BC6H_SF16:
        {
            typeless_format = DXGI_FORMAT_BC6H_TYPELESS;
            break;
        }
        case DXGI_FORMAT_BC7_UNORM:
        case DXGI_FORMAT_BC7_UNORM_SRGB:
        {
            typeless_format = DXGI_FORMAT_BC7_TYPELESS;
            break;
        }
    }

    return typeless_format;
}

DXGI_FORMAT Texture::getSRGBFormat(DXGI_FORMAT format)
{
    DXGI_FORMAT srgb_format = format;
    switch(format)
    {
        case DXGI_FORMAT_R8G8B8A8_UNORM:
        {
            srgb_format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
            break;
        }
        case DXGI_FORMAT_BC1_UNORM:
        {
            srgb_format = DXGI_FORMAT_BC1_UNORM_SRGB;
            break;
        }
        case DXGI_FORMAT_BC2_UNORM:
        {
            srgb_format = DXGI_FORMAT_BC2_UNORM_SRGB;
            break;
        }
        case DXGI_FORMAT_BC3_UNORM:
        {
            srgb_format = DXGI_FORMAT_BC3_UNORM_SRGB;
            break;
        }
        case DXGI_FORMAT_B8G8R8A8_UNORM:
        {
            srgb_format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
            break;
        }
        case DXGI_FORMAT_B8G8R8X8_UNORM:
        {
            srgb_format = DXGI_FORMAT_B8G8R8X8_UNORM_SRGB;
            break;
        }
        case DXGI_FORMAT_BC7_UNORM:
        {
            srgb_format = DXGI_FORMAT_BC7_UNORM_SRGB;
            break;
        }
    }

    return srgb_format;
}

DXGI_FORMAT Texture::getUAVComptableFormat(DXGI_FORMAT format)
{
     DXGI_FORMAT uav_format = format;

    switch(format)
    {
        case DXGI_FORMAT_R8G8B8A8_TYPELESS:
        case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
        case DXGI_FORMAT_B8G8R8A8_UNORM:
        case DXGI_FORMAT_B8G8R8X8_UNORM:
        case DXGI_FORMAT_B8G8R8A8_TYPELESS:
        case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
        case DXGI_FORMAT_B8G8R8X8_TYPELESS:
        case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
        {
            uav_format = DXGI_FORMAT_R8G8B8A8_UNORM;
            break;
        }
        case DXGI_FORMAT_R32_TYPELESS:
        case DXGI_FORMAT_D32_FLOAT:
        {
            uav_format = DXGI_FORMAT_R32_FLOAT;
            break;
        }
    }

    return uav_format;
}

void Texture::resize(uint32_t width, uint32_t height, uint32_t depth_or_array_size)
{
    if(this->d3d12_resource)
    {
        D3D12_RESOURCE_DESC resource_desc = this->d3d12_resource->GetDesc();
        resource_desc.Width = std::max(width, 1u);
        resource_desc.Height = std::max(height, 1u);
        resource_desc.DepthOrArraySize = depth_or_array_size;
        resource_desc.MipLevels = resource_desc.SampleDesc.Count > 1 ? 1 : 0;

        Microsoft::WRL::ComPtr<ID3D12Device2> d3d12_device = this->device.getD3D12Device();

        D3D12_HEAP_PROPERTIES heap_properties = {};
        heap_properties.Type = D3D12_HEAP_TYPE_DEFAULT;
        heap_properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        heap_properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        heap_properties.CreationNodeMask = 1;
        heap_properties.VisibleNodeMask = 1;

        throwIfFailed(d3d12_device->CreateCommittedResource(&heap_properties, D3D12_HEAP_FLAG_NONE, &resource_desc, D3D12_RESOURCE_STATE_COMMON, this->d3d12_clear_value.get(), IID_PPV_ARGS(&this->d3d12_resource)));

        this->d3d12_resource->SetName(this->resource_name.c_str());

        ResourceStateTracker::addGlobalResourceState(this->d3d12_resource.Get(), D3D12_RESOURCE_STATE_COMMON);

        this->createViews();
    }
}

D3D12_CPU_DESCRIPTOR_HANDLE Texture::getRenderTargetView() const
{
    return this->render_target_view.getDescriptorHandle();
}

D3D12_CPU_DESCRIPTOR_HANDLE Texture::getDepthStencilView() const
{
    return this->depth_stencil_view.getDescriptorHandle();
}

D3D12_CPU_DESCRIPTOR_HANDLE Texture::getShaderResourceView() const
{
    return this->shader_resource_view.getDescriptorHandle();
}

bool Texture::hasAlpha() const
{
    DXGI_FORMAT format = this->getD3D12ResourceDesc().Format;

    bool has_alpha = false;

    switch(format)
    {
        case DXGI_FORMAT_R32G32B32A32_TYPELESS:
        case DXGI_FORMAT_R32G32B32A32_FLOAT:
        case DXGI_FORMAT_R32G32B32A32_UINT:
        case DXGI_FORMAT_R32G32B32A32_SINT:
        case DXGI_FORMAT_R16G16B16A16_TYPELESS:
        case DXGI_FORMAT_R16G16B16A16_FLOAT:
        case DXGI_FORMAT_R16G16B16A16_UNORM:
        case DXGI_FORMAT_R16G16B16A16_UINT:
        case DXGI_FORMAT_R16G16B16A16_SNORM:
        case DXGI_FORMAT_R16G16B16A16_SINT:
        case DXGI_FORMAT_R10G10B10A2_TYPELESS:
        case DXGI_FORMAT_R10G10B10A2_UNORM:
        case DXGI_FORMAT_R10G10B10A2_UINT:
        case DXGI_FORMAT_R8G8B8A8_TYPELESS:
        case DXGI_FORMAT_R8G8B8A8_UNORM:
        case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
        case DXGI_FORMAT_R8G8B8A8_UINT:
        case DXGI_FORMAT_R8G8B8A8_SNORM:
        case DXGI_FORMAT_R8G8B8A8_SINT:
        case DXGI_FORMAT_BC1_TYPELESS:
        case DXGI_FORMAT_BC1_UNORM:
        case DXGI_FORMAT_BC1_UNORM_SRGB:
        case DXGI_FORMAT_BC2_TYPELESS:
        case DXGI_FORMAT_BC2_UNORM:
        case DXGI_FORMAT_BC2_UNORM_SRGB:
        case DXGI_FORMAT_BC3_TYPELESS:
        case DXGI_FORMAT_BC3_UNORM:
        case DXGI_FORMAT_BC3_UNORM_SRGB:
        case DXGI_FORMAT_B5G5R5A1_UNORM:
        case DXGI_FORMAT_B8G8R8A8_UNORM:
        case DXGI_FORMAT_B8G8R8X8_UNORM:
        case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
        case DXGI_FORMAT_B8G8R8A8_TYPELESS:
        case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
        case DXGI_FORMAT_B8G8R8X8_TYPELESS:
        case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
        case DXGI_FORMAT_BC6H_TYPELESS:
        case DXGI_FORMAT_BC7_TYPELESS:
        case DXGI_FORMAT_BC7_UNORM:
        case DXGI_FORMAT_BC7_UNORM_SRGB:
        case DXGI_FORMAT_A8P8:
        case DXGI_FORMAT_B4G4R4A4_UNORM:
        {
            has_alpha = true;
            break;
        }
    }

    return has_alpha;
}

size_t Texture::bitsPerPixel() const
{
    DXGI_FORMAT format = this->getD3D12ResourceDesc().Format;
    return DirectX::BitsPerPixel(format);
}

Texture::Texture(Device& device, const D3D12_RESOURCE_DESC& resource_desc, const D3D12_CLEAR_VALUE* clear_value)
    : Resource(device, resource_desc, clear_value)
{
    this->createViews();
}

Texture::Texture(Device& device, Microsoft::WRL::ComPtr<ID3D12Resource> resource, const D3D12_CLEAR_VALUE* clear_value)
    : Resource(device, resource, clear_value)
{
    this->createViews();
}

Texture::~Texture() {}

void Texture::createViews()
{
    if(this->d3d12_resource)
    {
        Microsoft::WRL::ComPtr<ID3D12Device2> d3d12_device = this->device.getD3D12Device();

        D3D12_RESOURCE_DESC desc = this->d3d12_resource->GetDesc();

        if((desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET) != 0 && this->checkRTVSupport())
        {
            this->render_target_view = this->device.allocateDescriptors(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
            d3d12_device->CreateRenderTargetView(this->d3d12_resource.Get(), nullptr,
                                                 this->render_target_view.getDescriptorHandle());
        }
        if((desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL) != 0 && this->checkDSVSupport())
        {
            this->depth_stencil_view = this->device.allocateDescriptors(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
            d3d12_device->CreateDepthStencilView(this->d3d12_resource.Get(), nullptr,
                                                 this->depth_stencil_view.getDescriptorHandle());
        }
        if((desc.Flags & D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE) == 0 && this->checkSRVSupport())
        {
            this->shader_resource_view = this->device.allocateDescriptors(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
            d3d12_device->CreateShaderResourceView(this->d3d12_resource.Get(), nullptr,
                                                   this->shader_resource_view.getDescriptorHandle());
        }
        if((desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS) != 0 && this->checkUAVSupport() && desc.DepthOrArraySize == 1)
        {
            this->unordered_access_view = this->device.allocateDescriptors(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, desc.MipLevels);
            for(int i = 0; i < desc.MipLevels; ++i)
            {
                auto uav_desc = this->getUAVDesc(desc, i);
                d3d12_device->CreateUnorderedAccessView(this->d3d12_resource.Get(), nullptr, &uav_desc,
                                                        this->unordered_access_view.getDescriptorHandle(i));
            }
        }
    }
}

D3D12_UNORDERED_ACCESS_VIEW_DESC Texture::getUAVDesc(const D3D12_RESOURCE_DESC& resource_desc, UINT mip_slice, UINT array_slice, UINT plane_slice)
{
    D3D12_UNORDERED_ACCESS_VIEW_DESC uav_desc = {};
    uav_desc.Format                           = resource_desc.Format;

    switch(resource_desc.Dimension)
    {
        case D3D12_RESOURCE_DIMENSION_TEXTURE1D:
        {
            if(resource_desc.DepthOrArraySize > 1)
            {
                uav_desc.ViewDimension                  = D3D12_UAV_DIMENSION_TEXTURE1DARRAY;
                uav_desc.Texture1DArray.ArraySize       = resource_desc.DepthOrArraySize - array_slice;
                uav_desc.Texture1DArray.FirstArraySlice = array_slice;
                uav_desc.Texture1DArray.MipSlice        = mip_slice;
            }
            else
            {
                uav_desc.ViewDimension      = D3D12_UAV_DIMENSION_TEXTURE1D;
                uav_desc.Texture1D.MipSlice = mip_slice;
            }
            break;
        }
        case D3D12_RESOURCE_DIMENSION_TEXTURE2D:
        {
            if(resource_desc.DepthOrArraySize > 1)
            {
                uav_desc.ViewDimension                  = D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
                uav_desc.Texture2DArray.ArraySize       = resource_desc.DepthOrArraySize - array_slice;
                uav_desc.Texture2DArray.FirstArraySlice = array_slice;
                uav_desc.Texture2DArray.PlaneSlice      = plane_slice;
                uav_desc.Texture2DArray.MipSlice        = mip_slice;
            }
            else
            {
                uav_desc.ViewDimension        = D3D12_UAV_DIMENSION_TEXTURE2D;
                uav_desc.Texture2D.PlaneSlice = plane_slice;
                uav_desc.Texture2D.MipSlice   = mip_slice;
            }
            break;
        }
        case D3D12_RESOURCE_DIMENSION_TEXTURE3D:
        {
            uav_desc.ViewDimension         = D3D12_UAV_DIMENSION_TEXTURE3D;
            uav_desc.Texture3D.WSize       = resource_desc.DepthOrArraySize - array_slice;
            uav_desc.Texture3D.FirstWSlice = array_slice;
            uav_desc.Texture3D.MipSlice    = mip_slice;
            break;
        }
        default:
        {
            throw std::exception("Invalid resource dimension.");
        }
    }

    return uav_desc;
}

D3D12_CPU_DESCRIPTOR_HANDLE Texture::getUnorderedAccessView(uint32_t mip) const
{
    return this->unordered_access_view.getDescriptorHandle(mip);
}

MakeTexture::MakeTexture(Device& device, const D3D12_RESOURCE_DESC& resource_desc, const D3D12_CLEAR_VALUE* clear_value)
    : Texture(device, resource_desc, clear_value)
{}

MakeTexture::MakeTexture(Device& device, Microsoft::WRL::ComPtr<ID3D12Resource> resource, const D3D12_CLEAR_VALUE* clear_value)
    : Texture(device, resource, clear_value)
{}

MakeTexture::~MakeTexture() {}

} // namespace gg;
