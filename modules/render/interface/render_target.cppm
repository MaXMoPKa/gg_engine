module;

#include <cstdint>
#include <memory>
#include <vector>

#include <DirectXMath.h>
#include <d3d12.h>

export module render:render_target;

namespace gg
{

class Texture;

export enum class AttachmentPoint
{
    COLOR_0,
    COLOR_1,
    COLOR_2,
    COLOR_3,
    COLOR_4,
    COLOR_5,
    COLOR_6,
    COLOR_7,
    DEPTH_STENCIL,
    COUNT
};

export class RenderTarget
{
public:
    RenderTarget();

    RenderTarget(const RenderTarget& other) = default;
    RenderTarget(RenderTarget&& other) = default;

    RenderTarget& operator=(const RenderTarget& other) = default;
    RenderTarget& operator=(RenderTarget&& other) = default;

    void attachTexture(AttachmentPoint attachment_point, std::shared_ptr<Texture> texture);
    [[nodiscard]] std::shared_ptr<Texture> getTexture(AttachmentPoint attachment_point) const;

    void resize(DirectX::XMUINT2 size);
    void resize(uint32_t width, uint32_t height);

    [[nodiscard]] DirectX::XMUINT2 getSize() const;
    [[nodiscard]] uint32_t getWidth() const;
    [[nodiscard]] uint32_t getHeight() const;

    [[nodiscard]] D3D12_VIEWPORT getViewport(DirectX::XMFLOAT2 scale = {1.0f, 1.0f}, DirectX::XMFLOAT2 bias = {0.0f, 0.0f},
                                             float min_depth = 0.0f, float max_depth = 1.0f) const;

    [[nodiscard]] const std::vector<std::shared_ptr<Texture>>& getTextures() const;

    [[nodiscard]] D3D12_RT_FORMAT_ARRAY getRenderTargetFormats() const;

    [[nodiscard]] DXGI_FORMAT getDepthStencilFormat() const;

    [[nodiscard]] DXGI_SAMPLE_DESC getSampleDesc() const;

    void reset();
    /*{
        this->textures = RenderTargetList(AttachmentPoint::COUNT);
    }*/
private:
    using RenderTargetList = std::vector<std::shared_ptr<Texture>>;
    RenderTargetList textures;
    DirectX::XMUINT2 size;
};

} // namespace gg;
