module;

#define NOMINMAX

#include <vector>
#include <memory>

#include <DirectXMath.h>
#include <d3d12.h>

module render;

import :render_target;

namespace gg
{

RenderTarget::RenderTarget()
    : textures(static_cast<uint8_t>(AttachmentPoint::COUNT))
    , size{0, 0}
{}

void RenderTarget::attachTexture(AttachmentPoint attachment_point, std::shared_ptr<Texture> texture)
{
    this->textures[static_cast<uint8_t>(attachment_point)] = texture;
    if(texture && texture->getD3D12Resource())
    {
        D3D12_RESOURCE_DESC desc = texture->getD3D12ResourceDesc();

        this->size.x = static_cast<uint32_t>(desc.Width);
        this->size.y = static_cast<uint32_t>(desc.Height);
    }
}

std::shared_ptr<Texture> RenderTarget::getTexture(AttachmentPoint attachment_point) const
{
    return this->textures[static_cast<uint8_t>(attachment_point)];
}

void RenderTarget::resize(DirectX::XMUINT2 size)
{
    this->size = size;
    for(std::shared_ptr<Texture> texture : this->textures)
    {
        if(texture)
        {
            texture->resize(size.x, size.y);
        }
    }
}

void RenderTarget::resize(uint32_t width, uint32_t height)
{
    this->resize(DirectX::XMUINT2(width, height));
}

DirectX::XMUINT2 RenderTarget::getSize() const
{
    return this->size;
}

uint32_t RenderTarget::getWidth() const
{
    return this->size.x;
}

uint32_t RenderTarget::getHeight() const
{
    return this->size.y;
}

D3D12_VIEWPORT RenderTarget::getViewport(DirectX::XMFLOAT2 scale, DirectX::XMFLOAT2 bias, float min_depth, float max_depth) const
{
    UINT64 width = 0;
    UINT height = 0;

    for(uint8_t i = static_cast<uint8_t>(AttachmentPoint::COLOR_0); i <= static_cast<uint8_t>(AttachmentPoint::COLOR_7); ++i)
    {
        std::shared_ptr<Texture> texture = this->textures[i];
        if(texture)
        {
            D3D12_RESOURCE_DESC desc = texture->getD3D12ResourceDesc();
            width = std::max(width, desc.Width);
            height = std::max(height, desc.Height);
        }
    }

    D3D12_VIEWPORT viewport = {width * bias.x,
                               height * bias.y,
                               width * scale.x,
                               height * scale.y,
                               min_depth,
                               max_depth};
    return viewport;
}

const std::vector<std::shared_ptr<Texture>>& RenderTarget::getTextures() const
{
    return this->textures;
}

D3D12_RT_FORMAT_ARRAY RenderTarget::getRenderTargetFormats() const
{
    D3D12_RT_FORMAT_ARRAY rtv_formats = {};

    for(uint8_t i = static_cast<uint8_t>(AttachmentPoint::COLOR_0); i <= static_cast<uint8_t>(AttachmentPoint::COLOR_7); ++i)
    {
        std::shared_ptr<Texture> texture = this->textures[i];
        if(texture)
        {
            rtv_formats.RTFormats[rtv_formats.NumRenderTargets++] = texture->getD3D12ResourceDesc().Format;
        }
    }

    return rtv_formats;
}

DXGI_FORMAT RenderTarget::getDepthStencilFormat() const
{
    DXGI_FORMAT dsv_format = DXGI_FORMAT_UNKNOWN;
    std::shared_ptr<Texture> depth_stencil_texture = this->textures[static_cast<uint8_t>(AttachmentPoint::DEPTH_STENCIL)];
    if(depth_stencil_texture)
    {
        dsv_format = depth_stencil_texture->getD3D12ResourceDesc().Format;
    }

    return dsv_format;
}

DXGI_SAMPLE_DESC RenderTarget::getSampleDesc() const
{
    DXGI_SAMPLE_DESC sample_desc = {1, 0};
    for(uint8_t i = static_cast<uint8_t>(AttachmentPoint::COLOR_0); i <= static_cast<uint8_t>(AttachmentPoint::COLOR_7); ++i)
    {
        std::shared_ptr<Texture> texture = this->textures[i];
        if(texture)
        {
            sample_desc = texture->getD3D12ResourceDesc().SampleDesc;
            break;
        }
    }

    return sample_desc;
}

void RenderTarget::reset()
{
    this->textures = RenderTargetList(static_cast<uint8_t>(AttachmentPoint::COUNT));
}

} // namespace gg;
