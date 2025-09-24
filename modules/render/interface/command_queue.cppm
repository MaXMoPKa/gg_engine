module;

#include <atomic>
#include <condition_variable>
#include <cstdint>

#include <d3d12.h>

#include <wrl.h>

export module render:command_queue;

import :thread_save_queue;

namespace gg
{
class CommandList;
class Device;

export class CommandQueue
{
public:
    [[nodiscard]] std::shared_ptr<CommandList> getCommandList();

    [[nodiscard]] uint64_t executeCommandList(std::shared_ptr<CommandList> command_list);
    [[nodiscard]] uint64_t executeCommandLists(const std::vector<std::shared_ptr<CommandList>>& command_lists);

    [[nodiscard]] uint64_t signal();
    [[nodiscard]] bool isFenceComplete(uint64_t fence_value);
    void waitForFenceValue(uint64_t fence_value);
    void flush();

    void wait(const CommandQueue& other);

    [[nodiscard]] Microsoft::WRL::ComPtr<ID3D12CommandQueue> getD3D12CommandQueue() const;

protected:
    friend class std::default_delete<CommandQueue>;

    CommandQueue(Device& device, D3D12_COMMAND_LIST_TYPE);
    virtual ~CommandQueue();

private:
    void processInFlightCommandLists();

private:
    using CommandListEntry = std::tuple<uint64_t, std::shared_ptr<CommandList>>;

    Device& device;
    D3D12_COMMAND_LIST_TYPE d3d12_command_list_type;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> d3d12_command_queue;
    Microsoft::WRL::ComPtr<ID3D12Fence> d3d12_fence;
    std::atomic_uint64_t fence_value;

    ThreadSaveQueue<CommandListEntry> in_flight_command_lists;
    ThreadSaveQueue<std::shared_ptr<CommandList>> available_command_lists;

    std::thread process_in_flight_command_lists_thread;
    std::atomic_bool is_process_in_flight_command_lists;
    std::mutex process_in_flight_command_lists_thread_mutex;
    std::condition_variable process_in_flight_command_lists_thread_cv;
};

export class MakeCommandQueue : public CommandQueue
{
public:
    MakeCommandQueue(Device& device, D3D12_COMMAND_LIST_TYPE type);
    ~MakeCommandQueue() override;
};

}
