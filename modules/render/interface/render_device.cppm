module;

#include <memory>

#include <d3d12.h>

#include <wrl.h>

export module gg.render.device;

import gg.render.helpers;
import gg.render.adapter;

export namespace gg
{

class Device
{
public:
    static void enableDebugLayer();

    static std::shared_ptr<Device> create(std::shared_ptr<Adapter> adapter = nullptr);

protected:
    Device(std::shared_ptr<Adapter> adapter);
    virtual ~Device() = default;

private:
    Microsoft::WRL::ComPtr<ID3D12Device2> device;

    std::shared_ptr<Adapter> adapter;
};

} // namespace gg;
