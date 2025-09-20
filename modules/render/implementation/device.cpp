module;

#include <memory>
#include <cassert>

#include <dxgi1_6.h>
#include <d3d12.h>

#include <wrl.h>

module render;

import :device;
import :adapter;
import :command_queue;
import gg.render.helpers;

namespace gg
{
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

    CommandQueue& Device::getCommandQueue(D3D12_COMMAND_LIST_TYPE type)
    {
        CommandQueue* command_queue;
        switch(type)
        {
            case D3D12_COMMAND_LIST_TYPE_DIRECT:
            {
                command_queue = direct_command_queue.get();
                break;
            }
            case D3D12_COMMAND_LIST_TYPE_COPY:
            {
                command_queue = copy_command_queue.get();
                break;
            }
            case D3D12_COMMAND_LIST_TYPE_COMPUTE:
            {
                command_queue = compute_command_queue.get();
                break;
            }
            default:
            {
                assert(false && "Invalid command queue type.");
            }
        }

        return *command_queue;
    }

    Device::Device(std::shared_ptr<Adapter> adapter)
        : adapter{adapter}
    {
        using namespace Microsoft::WRL;

        if(!this->adapter)
        {
            this->adapter = Adapter::Create();
        }

        ComPtr<IDXGIAdapter4> dxgi_adapter = this->adapter->getDXGIAdapter();

        throwIfFailed(D3D12CreateDevice(dxgi_adapter.Get(), D3D_FEATURE_LEVEL_12_2, IID_PPV_ARGS(&this->device)));

        this->direct_command_queue = std::make_unique<MakeCommandQueue>(*this, D3D12_COMMAND_LIST_TYPE_DIRECT);
        this->compute_command_queue = std::make_unique<MakeCommandQueue>(*this, D3D12_COMMAND_LIST_TYPE_COMPUTE);
        this->copy_command_queue = std::make_unique<MakeCommandQueue>(*this, D3D12_COMMAND_LIST_TYPE_COPY);

        for(int i = 0; i < D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES; ++i)
        {
            this->descriptor_allocators[i] = std::make_unique<MakeDescriptorAllocator>(*this, static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(i));
        }

        D3D12_FEATURE_DATA_ROOT_SIGNATURE feature_data;
        feature_data.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;
        if(FAILED(this->device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &feature_data, sizeof(D3D12_FEATURE_DATA_ROOT_SIGNATURE))))
        {
            feature_data.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
        }
        this->highest_root_signature_version = feature_data.HighestVersion;
    }

    Device::~Device() {}

    MakeDevice::MakeDevice(std::shared_ptr<Adapter> adapter)
        : Device(adapter)
    {}

    MakeDevice::~MakeDevice() {}

} // namespace gg;
