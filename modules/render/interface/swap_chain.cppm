module;

#include <cstdint>
#include <memory>

#include <d3d12.h>

#include <dxgiformat.h>
#include <dxgi1_5.h>

#include <windows.h>
#include <wrl.h>

export module render:swap_chain;

import :render_target;

namespace gg
{

class Texture;
class Device;
class CommandQueue;

export class SwapChain
{
public:
    static const UINT buffer_count = 3;

    [[nodiscard]] inline bool isFullscreen() const
    {
        return this->fullscreen;
    }

    void setFullscreen(bool fullscreen);

    inline void toggleFullscreen()
    {
        this->setFullscreen(!this->fullscreen);
    }

    inline void setVSync(bool v_sync)
    {
        this->v_sync = v_sync;
    }

    [[nodiscard]] inline bool getVSync() const
    {
        return this->v_sync;
    }

    inline void toggleVSync()
    {
        this->setVSync(!this->v_sync);
    }

    [[nodiscard]] inline bool isTearingSupported() const
    {
        return this->tearing_supported;
    }

    void waitForSwapChain();

    void resize(uint32_t width, uint32_t height);

    [[nodiscard]] const RenderTarget& getRenderTarget() const;

    [[nodiscard]] UINT present(const std::shared_ptr<Texture>& texture = nullptr);

    [[nodiscard]] inline DXGI_FORMAT getRenderTargetFormat() const
    {
        return this->render_target_format;
    }

    Microsoft::WRL::ComPtr<IDXGISwapChain4> getDXGISwapChain() const
    {
        return this->swap_chain;
    }

protected:
    SwapChain(Device& device, HWND window_handle, DXGI_FORMAT render_target_format = DXGI_FORMAT_R10G10B10A2_UNORM);
    virtual ~SwapChain();

    void updateRenderTargetViews();
private:
    Device& device;
    CommandQueue& command_queue;
    Microsoft::WRL::ComPtr<IDXGISwapChain4> swap_chain;
    std::shared_ptr<Texture> back_buffer_textures[buffer_count];
    mutable RenderTarget render_target;

    UINT current_back_buffer_index;
    UINT64 fence_values[buffer_count];

    HANDLE frame_latency_waitable_objects_handle;

    HWND window_handle;

    uint32_t width;
    uint32_t height;

    DXGI_FORMAT render_target_format;

    bool v_sync;

    bool tearing_supported;

    bool fullscreen;
};

export class MakeSwapChain : public SwapChain
{
public:
    MakeSwapChain(Device& device, HWND window_handle, DXGI_FORMAT back_buffer_format = DXGI_FORMAT_R10G10B10A2_UNORM);
    ~MakeSwapChain() override;
};

} // namespace gg;
