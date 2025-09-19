module;

#include <atomic>
#include <condition_variable>
#include <cstdint>

#include <d3d12.h>

#include <wrl.h>

export module render:command_queue;

import gg.render.thread_save_queue;

namespace gg
{
class CommandList;
class Device;

export class CommandQueue
{
public:

    std::shared_ptr<CommandList> getCommandList();

    uint64_t executeCommandList(std::shared_ptr<CommandList> command_list);
    uint64_t executeCommandLists(const std::vector<std::shared_ptr<CommandList>>& command_lists);

    uint64_t signal();
    bool isFenceComplete(uint64_t fence_value);
    void waitForFenceValue(uint64_t fence_value);
    void flush();

    void wait(const CommandQueue& other);

    Microsoft::WRL::ComPtr<ID3D12CommandQueue> getD3D12CommandQueue() const;

protected:
    friend class std::default_delete<CommandQueue>;

    CommandQueue(Device& device, D3D12_COMMAND_LIST_TYPE);
    virtual ~CommandQueue();

private:
    void processInFlightCommandLists();

private:
    using CommandListEntry = std::tuple<uint64_t, std::shared_ptr<CommandList>>;

    Device& device;
    D3D12_COMMAND_LIST_TYPE command_list_type;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> command_queue;
    Microsoft::WRL::ComPtr<ID3D12Fence> fence;
    std::atomic_uint64_t fence_value;

    ThreadSaveQueue<CommandListEntry> in_flight_command_lists;
    ThreadSaveQueue<std::shared_ptr<CommandList>> available_command_lists;

    std::thread process_in_flight_command_lists_thread;
    std::atomic_bool is_process_in_flight_command_lists;
    std::mutex process_in_flight_command_lists_thread_mutex;
    std::condition_variable process_in_flight_command_lists_thread_cv;
};

}
