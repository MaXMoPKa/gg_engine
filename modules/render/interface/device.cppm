module;

#include <memory>

#include <d3d12.h>

#include <wrl.h>

export module render:device;

import gg.render.helpers;

namespace gg
{

class Adapter;
class CommandQueue;
class DescriptorAllocator;
class DescriptorAllocation;
class SwapChain;
class Texture;

export class Device
{
public:
    static void enableDebugLayer();

    [[nodiscard]] static std::shared_ptr<Device> create(std::shared_ptr<Adapter> adapter = nullptr);

public:
    DescriptorAllocation allocateDescriptors(D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t num_descriptors = 1);

    [[nodiscard]] std::shared_ptr<SwapChain> createSwapChain(HWND hwnd, DXGI_FORMAT back_buffer_format = DXGI_FORMAT_R10G10B10A2_UNORM);

    [[nodiscard]] std::shared_ptr<Texture> createTexture(const D3D12_RESOURCE_DESC& resource_desc, const D3D12_CLEAR_VALUE* clear_value = nullptr);
    [[nodiscard]] std::shared_ptr<Texture> createTexture(Microsoft::WRL::ComPtr<ID3D12Resource> resource, const D3D12_CLEAR_VALUE* clear_value = nullptr);

    [[nodiscard]] inline Microsoft::WRL::ComPtr<ID3D12Device2> getD3D12Device() const
    {
        return this->device;
    }

    [[nodiscard]] inline std::shared_ptr<Adapter> getAdapter() const
    {
        return this->adapter;
    }

    [[nodiscard]] CommandQueue& getCommandQueue(D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT);

    void flush();

    void releaseStaleDescriptors();

protected:
    Device(std::shared_ptr<Adapter> adapter);
    virtual ~Device();

private:
    Microsoft::WRL::ComPtr<ID3D12Device2> device;

    std::shared_ptr<Adapter> adapter;

    std::unique_ptr<CommandQueue> direct_command_queue;
    std::unique_ptr<CommandQueue> compute_command_queue;
    std::unique_ptr<CommandQueue> copy_command_queue;

    std::unique_ptr<DescriptorAllocator> descriptor_allocators[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES];

    D3D_ROOT_SIGNATURE_VERSION highest_root_signature_version;
};

export class MakeDevice : public Device
{
public:
    MakeDevice(std::shared_ptr<Adapter> adapter);
    ~MakeDevice() override;
};

} // namespace gg;
