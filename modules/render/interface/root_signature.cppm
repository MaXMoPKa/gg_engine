module;

#include <cstdint>
#include <memory>

#include <d3d12.h>

#include <wrl.h>

export module render:root_signature;

namespace gg
{

class Device;

export class RootSignature
{
public:
    [[nodiscard]] inline Microsoft::WRL::ComPtr<ID3D12RootSignature> getD3D12RootSignature() const
    {
        return this->d3d12_root_signature;
    }

    [[nodiscard]] inline const D3D12_ROOT_SIGNATURE_DESC1& getRootSignatureDesc() const
    {
        return this->root_signature_desc;
    }

    [[nodiscard]] uint32_t getDescriptorTableBitMask(D3D12_DESCRIPTOR_HEAP_TYPE descriptor_heap_type);
    [[nodiscard]] uint32_t getNumDescriptors(uint32_t root_index) const;
protected:
    friend class std::default_delete<RootSignature>;

    RootSignature(Device& device, const D3D12_ROOT_SIGNATURE_DESC1& desc);
    virtual ~RootSignature();
private:
    void destroy();
    void setRootSignatureDesc(const D3D12_ROOT_SIGNATURE_DESC1& desc);
private:
    Device& device;
    D3D12_ROOT_SIGNATURE_DESC1 root_signature_desc;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> d3d12_root_signature;

    uint32_t num_descriptors_per_table[32];

    uint32_t sampler_table_bit_mask;

    uint32_t descriptor_table_bits_mask;
};

} // namespace gg;
