module;

#include <d3d12.h>

#include <wrl.h>

module render;

import :pipline_state_object;
import :device;
import gg.render.helpers;

namespace gg
{

PiplineStateObject::PiplineStateObject(Device& device, const D3D12_PIPELINE_STATE_STREAM_DESC& desc)
    : device(device)
{
    Microsoft::WRL::ComPtr<ID3D12Device2> d3d12_device = this->device.getD3D12Device();
    throwIfFailed(d3d12_device->CreatePipelineState(&desc, IID_PPV_ARGS(&this->d3d12_pipeline_state)));
}

} // namespace gg;
