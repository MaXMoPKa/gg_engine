module;

#include <d3d12.h>
#include <wrl.h>

export module gg.render.device;

export namespace gg
{
class RenderDevice
{
public:
    RenderDevice()
    {
        #if defined (_DEBUG)
            Microsoft::WRL::ComPtr<ID3D12Debug> debug_controller;
            if(SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_controller))))
            {
                debug_controller->EnableDebugLayer();
            }
        #endif
    }

    ~RenderDevice() = default;

private:
};

} // namespace gg;
