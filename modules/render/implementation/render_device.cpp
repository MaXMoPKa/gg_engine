module;

#include <memory>

#include <dxgi1_6.h>
#include <d3d12.h>

#include <wrl.h>

module gg.render.device;

import gg.render.helpers;

namespace gg
{

class MakeDevice : public Device
{
public:
    MakeDevice(std::shared_ptr<Adapter> adapter)
        : Device(adapter)
    {}

    ~MakeDevice() override {}
};

void Device::enableDebugLayer()
{
    Microsoft::WRL::ComPtr<ID3D12Debug> debug_interface;
    throwIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_interface)));
    debug_interface->EnableDebugLayer();
}

std::shared_ptr<Device> Device::create(std::shared_ptr<Adapter> adapter)
{
   return std::make_shared<MakeDevice>(adapter);
}

Device::Device(std::shared_ptr<Adapter> adapter)
    : adapter{adapter}
{
    using namespace Microsoft::WRL;

    if(!this->adapter)
    {
        this->adapter = Adapter::Create();
    }

    ComPtr<IDXGIAdapter4> dxgi_adapter = adapter->getDXGIAdapter();

    throwIfFailed(D3D12CreateDevice(dxgi_adapter.Get(), D3D_FEATURE_LEVEL_12_2, IID_PPV_ARGS(&device)));
}

} // namespace gg;
