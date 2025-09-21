module;

#include <string>
#include <memory>

#include <d3d12.h>

#include <wrl.h>

export module render:resource;

namespace gg
{

class Device;

export class Resource
{
public:
    [[nodiscard]] inline Device& getDevice() const
    {
        return this->device;
    }

    [[nodiscard]] inline Microsoft::WRL::ComPtr<ID3D12Resource> getD3D12Resource() const
    {
        return this->d3d12_resource;
    }

    [[nodiscard]] inline D3D12_RESOURCE_DESC getD3D12ResourceDesc() const
    {
        D3D12_RESOURCE_DESC resource_desc = {};
        if(this->d3d12_resource)
        {
            resource_desc = this->d3d12_resource->GetDesc();
        }

        return resource_desc;
    }

    void setName(const std::wstring& name);
    [[nodiscard]] inline const std::wstring& getName() const
    {
        return this->resource_name;
    }

    [[nodiscard]] bool checkFormatSupport(D3D12_FORMAT_SUPPORT1 format_support) const;
    [[nodiscard]] bool checkFormatSupport(D3D12_FORMAT_SUPPORT2 format_support) const;
protected:
    Resource(Device& device, const D3D12_RESOURCE_DESC& resource_desc, const D3D12_CLEAR_VALUE* clear_value = nullptr);
    Resource(Device& device, Microsoft::WRL::ComPtr<ID3D12Resource> resource, const D3D12_CLEAR_VALUE* clear_value = nullptr);

    virtual ~Resource() = default;
protected:
    Device& device;

    Microsoft::WRL::ComPtr<ID3D12Resource> d3d12_resource;
    D3D12_FEATURE_DATA_FORMAT_SUPPORT format_support;
    std::unique_ptr<D3D12_CLEAR_VALUE> d3d12_clear_value;
    std::wstring resource_name;
private:
    void checkFeatureSupport();
};

} // namespace gg;
