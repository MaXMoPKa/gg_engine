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

export class Device
{
public:
    static void enableDebugLayer();

    [[nodiscard]] static std::shared_ptr<Device> create(std::shared_ptr<Adapter> adapter = nullptr);

public:
    [[nodiscard]] inline Microsoft::WRL::ComPtr<ID3D12Device2> getD3D12Device() const
    {
        return this->device;
    }

    [[nodiscard]] CommandQueue& getCommandQueue(D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT);

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
