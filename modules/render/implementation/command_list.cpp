module;

#include <memory>

#include <d3d12.h>

#include <wrl/client.h>

module render;

import :command_list;
import :upload_buffer;

namespace gg
{
    void CommandList::flushResourceBarriers()
    {
        this->resource_state_tracker->flushResourceBarriers(shared_from_this());
    }

    CommandList::CommandList(Device& device, D3D12_COMMAND_LIST_TYPE type)
        : device{device}
        , command_list_type{type}
        , root_signature{nullptr}
        , pipeline_state{nullptr}
    {
        Microsoft::WRL::ComPtr<ID3D12Device2> d3d12_device = this->device.getD3D12Device();

        throwIfFailed(d3d12_device->CreateCommandAllocator(this->command_list_type, IID_PPV_ARGS(&this->command_allocator)));
        throwIfFailed(d3d12_device->CreateCommandList(0, this->command_list_type, this->command_allocator.Get(), nullptr, IID_PPV_ARGS(&this->command_list)));

        this->upload_buffer = std::make_unique<MakeUploadBuffer>(this->device);

        this->resource_state_tracker = std::make_unique<ResourceStateTracker>();

        for(int i = 0; i < D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES; ++i)
        {
            dynamic_descriptor_heap[i] = std::make_unique<DynamicDescriptorHeap>(device, static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(i));
            descriptor_heaps[i] = nullptr;
        }
    }

    CommandList::~CommandList() {}

    bool CommandList::close(const std::shared_ptr<CommandList>& pending_command_list)
    {
        this->flushResourceBarriers();
        this->command_list->Close();
        uint32_t num_pending_barriers = this->resource_state_tracker->flushPendingResourceBarriers(pending_command_list);
        this->resource_state_tracker->commitFinalResourceStates();
        return num_pending_barriers > 0;
    }

    void CommandList::close()
    {
        this->flushResourceBarriers();
        this->command_list->Close();
    }

    void CommandList::reset()
    {
        throwIfFailed(this->command_allocator->Reset());
        throwIfFailed(this->command_list->Reset(this->command_allocator.Get(), nullptr));

        this->resource_state_tracker->reset();

        this->upload_buffer->reset();

        this->releaseTrackedObjects();

        for(int i = 0; i < D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES; ++i)
        {
            this->dynamic_descriptor_heap[i]->reset();
            this->descriptor_heaps[i] = nullptr;
        }

        this->root_signature = nullptr;
        this->pipeline_state = nullptr;
        this->compute_command_list = nullptr;
    }

    void CommandList::releaseTrackedObjects()
    {
        this->tracked_objects.clear();
    }

    MakeCommandList::MakeCommandList(Device& device, D3D12_COMMAND_LIST_TYPE type)
        : CommandList(device, type)
    {}

    MakeCommandList::~MakeCommandList() {}

} // namespace gg;
