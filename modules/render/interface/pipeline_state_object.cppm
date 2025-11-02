module;

#include <d3d12.h>

#include <wrl.h>

export module render:pipline_state_object;

namespace gg
{

class Device;

export class PiplineStateObject
{
public:
    Microsoft::WRL::ComPtr<ID3D12PipelineState> getD3D12PiplineState() const
    {
        return this->d3d12_pipeline_state;
    }
protected:
    PiplineStateObject(Device& device, const D3D12_PIPELINE_STATE_STREAM_DESC& desc);
    virtual ~PiplineStateObject() = default;
private:
    Device& device;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> d3d12_pipeline_state;
};

} // namespace gg;
