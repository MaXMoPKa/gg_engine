module;

#include <cstdint>

#include <d3d12.h>

#include <wrl.h>

export module render:texture;

import :descriptor_allocation;
import :resource;

namespace gg
{

class Device;

export class Texture : public Resource
{
public:
    [[nodiscard]] static bool isUAVCompatibleFormat(DXGI_FORMAT format);
    [[nodiscard]] static bool isSRGBFormat(DXGI_FORMAT format);
    [[nodiscard]] static bool isBGRFormat(DXGI_FORMAT format);
    [[nodiscard]] static bool isDepthFormat(DXGI_FORMAT format);

    [[nodiscard]] static DXGI_FORMAT getTypelessFormat(DXGI_FORMAT format);
    [[nodiscard]] static DXGI_FORMAT getSRGBFormat(DXGI_FORMAT format);
    [[nodiscard]] static DXGI_FORMAT getUAVComptableFormat(DXGI_FORMAT format);

public:
    void resize(uint32_t width, uint32_t height, uint32_t depth_or_array_size = 1);

    [[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE getRenderTargetView() const;

    [[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE getDepthStencilView() const;

    [[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE getShaderResourceView() const;

    [[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE getUnorderedAccessView(uint32_t mip) const;

    [[nodiscard]] inline bool checkSRVSupport() const
    {
        return this->checkFormatSupport(D3D12_FORMAT_SUPPORT1_SHADER_SAMPLE);
    }

    [[nodiscard]] inline bool checkRTVSupport() const
    {
        return this->checkFormatSupport(D3D12_FORMAT_SUPPORT1_RENDER_TARGET);
    }

    [[nodiscard]] inline bool checkUAVSupport() const
    {
        return this->checkFormatSupport(D3D12_FORMAT_SUPPORT1_TYPED_UNORDERED_ACCESS_VIEW) &&
               this->checkFormatSupport(D3D12_FORMAT_SUPPORT2_UAV_TYPED_LOAD) &&
               this->checkFormatSupport(D3D12_FORMAT_SUPPORT2_UAV_TYPED_STORE);
    }

    [[nodiscard]] inline bool checkDSVSupport() const
    {
        return this->checkFormatSupport(D3D12_FORMAT_SUPPORT1_DEPTH_STENCIL);
    }

    [[nodiscard]] bool hasAlpha() const;

    [[nodiscard]] size_t bitsPerPixel() const;

protected:
    Texture(Device& device, const D3D12_RESOURCE_DESC& resource_desc, const D3D12_CLEAR_VALUE* clear_value = nullptr);
    Texture(Device& device, Microsoft::WRL::ComPtr<ID3D12Resource> resource, const D3D12_CLEAR_VALUE* clear_value = nullptr);
    virtual ~Texture();

    void createViews();
private:
    D3D12_UNORDERED_ACCESS_VIEW_DESC getUAVDesc(const D3D12_RESOURCE_DESC& resource_desc, UINT mip_slice, UINT array_slice = 0, UINT plane_slice = 0);
private:
    DescriptorAllocation render_target_view;
    DescriptorAllocation depth_stencil_view;
    DescriptorAllocation shader_resource_view;
    DescriptorAllocation unordered_access_view;
};

export class MakeTexture : public Texture
{
public:
    MakeTexture(Device& device, const D3D12_RESOURCE_DESC& resource_desc, const D3D12_CLEAR_VALUE* clear_value);
    MakeTexture(Device& device, Microsoft::WRL::ComPtr<ID3D12Resource> resource, const D3D12_CLEAR_VALUE* clear_value);
    ~MakeTexture() override;
};

} // namespace gg;
