module;

#include <string>
#include <vector>
#include <memory>

#include <wrl.h>

#include <d3d12.h>
#include <dxgi1_6.h>

export module render:adapter;

namespace gg
{

export class Adapter
{
public:
    using AdapterList = std::vector<std::shared_ptr<Adapter>>;

public:
    [[nodiscard]] static AdapterList getAdapters(DXGI_GPU_PREFERENCE gpu_preference = DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE);

    [[nodiscard]] static std::shared_ptr<Adapter> create(DXGI_GPU_PREFERENCE gpu_preference = DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE);

    [[nodiscard]] inline Microsoft::WRL::ComPtr<IDXGIAdapter4> getDXGIAdapter() const
    {
        return this->adapter;
    }

    [[nodiscard]] inline const std::wstring getDescriptor() const
    {
        return this->adapter_descriptor.Description;
    }
protected:
    Adapter(Microsoft::WRL::ComPtr<IDXGIAdapter4> dxgi_adapter);
    virtual ~Adapter() = default;
private:
    Microsoft::WRL::ComPtr<IDXGIAdapter4> adapter;
    DXGI_ADAPTER_DESC3                    adapter_descriptor;
};

export class MakeAdapter : public Adapter
{
public:
    MakeAdapter(Microsoft::WRL::ComPtr<IDXGIAdapter4> dxgi_adapter);
    ~MakeAdapter() override;
};

} // namespace gg;
