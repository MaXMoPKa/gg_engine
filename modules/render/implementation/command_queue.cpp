module;

#include <cstdio>
#include <thread>
#include <mutex>
#include <cassert>

#include <d3d12.h>

#include <wrl.h>

module render;

import :command_queue;
import :device;
import :command_list;
import :resource_state_tracker;
import gg.render.helpers;

namespace gg
{

    std::shared_ptr<CommandList> CommandQueue::getCommandList()
    {
        std::shared_ptr<CommandList> command_list;

        if(!available_command_lists.empty())
        {
            available_command_lists.try_pop(command_list);
        }
        else
        {
            command_list = std::make_shared<MakeCommandList>(device, command_list_type);
        }

        return command_list;
    }

    uint64_t CommandQueue::executeCommandList(std::shared_ptr<CommandList> command_list)
    {
        return this->executeCommandLists(std::vector<std::shared_ptr<CommandList>>({command_list}));
    }

    uint64_t CommandQueue::executeCommandLists(const std::vector<std::shared_ptr<CommandList>>& command_lists)
    {
        ResourceStateTracker::lock();

        std::vector<std::shared_ptr<CommandList>> to_be_queued;
        to_be_queued.reserve(command_lists.size() * 2);

        std::vector<std::shared_ptr<CommandList>> generate_mips_command_lists;
        generate_mips_command_lists.reserve(command_lists.size());

        std::vector<ID3D12CommandList*> d3d12_command_lists;
        d3d12_command_lists.reserve(command_lists.size() * 2);

        for(std::shared_ptr<CommandList> command_list : command_lists)
        {
            std::shared_ptr<CommandList> pending_command_list = getCommandList();
            bool has_pending_barriers = command_list->close(pending_command_list);
            pending_command_list->close();

            if(has_pending_barriers)
            {
                d3d12_command_lists.push_back(pending_command_list->getD3D12CommandList().Get());
            }
            d3d12_command_lists.push_back(command_list->getD3D12CommandList().Get());

            to_be_queued.push_back(pending_command_list);
            to_be_queued.push_back(command_list);

            std::shared_ptr<CommandList> generate_mips_command_list = command_list->getGenerateMipsCommandList();
            if(generate_mips_command_list)
            {
                generate_mips_command_lists.push_back(generate_mips_command_list);
            }
        }

        UINT num_command_lists = static_cast<UINT>(d3d12_command_lists.size());
        this->command_queue->ExecuteCommandLists(num_command_lists, d3d12_command_lists.data());
        uint64_t fence_value = signal();

        ResourceStateTracker::unlock();

        for(std::shared_ptr<CommandList> command_list : to_be_queued)
        {
            in_flight_command_lists.push({fence_value, command_list});
        }

        if(generate_mips_command_lists.size() > 0)
        {
            CommandQueue& compute_queue = device.getCommandQueue(D3D12_COMMAND_LIST_TYPE_COMPUTE);
            compute_queue.wait(*this);
            compute_queue.executeCommandLists(generate_mips_command_lists);
        }

        return fence_value;
    }

    uint64_t CommandQueue::signal()
    {
        uint64_t fence_value = ++(this->fence_value);
        command_queue->Signal(fence.Get(), fence_value);
        return fence_value;
    }

    bool CommandQueue::isFenceComplete(uint64_t fence_value)
    {
        return this->fence->GetCompletedValue() >= fence_value;
    }

    void CommandQueue::waitForFenceValue(uint64_t fence_value)
    {
        if(isFenceComplete(fence_value))
        {
            HANDLE event = ::CreateEvent(NULL, FALSE, FALSE, NULL);
            if(event)
            {
                fence->SetEventOnCompletion(fence_value, event);
                ::WaitForSingleObject(event, DWORD_MAX);

                ::CloseHandle(event);
            }
        }
    }

    void CommandQueue::flush()
    {
        std::unique_lock<std::mutex> lock(process_in_flight_command_lists_thread_mutex);
        process_in_flight_command_lists_thread_cv.wait(lock, [this]{return in_flight_command_lists.empty();});
        waitForFenceValue(fence_value);
    }

    void CommandQueue::wait(const CommandQueue& other)
    {
        this->command_queue->Wait(other.fence.Get(), other.fence_value);
    }

    Microsoft::WRL::ComPtr<ID3D12CommandQueue> CommandQueue::getD3D12CommandQueue() const
    {
        return command_queue;
    }

    CommandQueue::CommandQueue(Device& device, D3D12_COMMAND_LIST_TYPE type)
        : device{device}
        , command_list_type{type}
    {
        Microsoft::WRL::ComPtr<ID3D12Device2> d3d12_device = device.getD3D12Device();

        D3D12_COMMAND_QUEUE_DESC desc = {};
        desc.Type = type;
        desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
        desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        desc.NodeMask = 0;

        throwIfFailed(d3d12_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&command_queue)));
        throwIfFailed(d3d12_device->CreateFence(fence_value, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));

        switch(type)
        {
            case D3D12_COMMAND_LIST_TYPE_COPY:
            {
                command_queue->SetName(L"Copy Command Queue");
                break;
            }
            case D3D12_COMMAND_LIST_TYPE_COMPUTE:
            {
                command_queue->SetName(L"Compute Command Queue");
                break;
            }
            case D3D12_COMMAND_LIST_TYPE_DIRECT:
            {
                command_queue->SetName(L"Direct Command Queue");
                break;
            }
            case D3D12_COMMAND_LIST_TYPE_BUNDLE:
            case D3D12_COMMAND_LIST_TYPE_VIDEO_DECODE:
            case D3D12_COMMAND_LIST_TYPE_VIDEO_PROCESS:
            case D3D12_COMMAND_LIST_TYPE_VIDEO_ENCODE:
            default:
            {
                assert(false && "Invalid command list type.");
            }
        }

        char thread_name[256];
        sprintf_s(thread_name, "ProcessInFlightCommandLists ");
        switch(type)
        {
            case D3D12_COMMAND_LIST_TYPE_COPY:
            {
                strcat_s(thread_name, "(Copy)");
                break;
            }
            case D3D12_COMMAND_LIST_TYPE_COMPUTE:
            {
                strcat_s(thread_name, "(Compute)");
                break;
            }
            case D3D12_COMMAND_LIST_TYPE_DIRECT:
            {
                strcat_s(thread_name, "(Direct)");
                break;
            }
            case D3D12_COMMAND_LIST_TYPE_BUNDLE:
            case D3D12_COMMAND_LIST_TYPE_VIDEO_DECODE:
            case D3D12_COMMAND_LIST_TYPE_VIDEO_PROCESS:
            case D3D12_COMMAND_LIST_TYPE_VIDEO_ENCODE:
            default:
            {
                assert(false && "Invalid command list type.");
            }

        }

        process_in_flight_command_lists_thread = std::thread(&CommandQueue::processInFlightCommandLists, this);
        setThreadName(process_in_flight_command_lists_thread, thread_name);
    }

    CommandQueue::~CommandQueue()
    {
        is_process_in_flight_command_lists = false;
        process_in_flight_command_lists_thread.join();
    }

    void CommandQueue::processInFlightCommandLists()
    {
        std::unique_lock<std::mutex> lock(process_in_flight_command_lists_thread_mutex, std::defer_lock);

        while(is_process_in_flight_command_lists)
        {
            CommandListEntry command_list_entry;

            lock.lock();
            while(in_flight_command_lists.try_pop(command_list_entry))
            {
                uint64_t fence_value = std::get<0>(command_list_entry);
                std::shared_ptr<CommandList> command_list = std::get<1>(command_list_entry);

                waitForFenceValue(fence_value);

                command_list->reset();

                available_command_lists.push(command_list);
            }
            lock.unlock();
            process_in_flight_command_lists_thread_cv.notify_one();

            std::this_thread::yield();
        }
    }
} // namespace gg;
