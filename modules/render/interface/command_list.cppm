module;

#include <vector>
#include <memory>

#include <d3d12.h>

#include <wrl.h>

export module render:command_list;

namespace gg
{

class Device;
class ResourceStateTracker;
class DynamicDescriptorHeap;
class UploadBuffer;

export class CommandList : public std::enable_shared_from_this<CommandList>
{
public:
    [[nodiscard]] inline Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> getD3D12CommandList() const
    {
        return this->command_list;
    }

    void flushResourceBarriers();

protected:
    friend class CommandQueue;
    friend class std::default_delete<CommandList>;

    CommandList(Device& device, D3D12_COMMAND_LIST_TYPE type);
    virtual ~CommandList();

    bool close(const std::shared_ptr<CommandList>& pending_command_list);
    void close();

    void reset();

    void releaseTrackedObjects();

    [[nodiscard]] inline std::shared_ptr<CommandList> getGenerateMipsCommandList() const
    {
        return this->compute_command_list;
    }

private:
    Device& device;
    D3D12_COMMAND_LIST_TYPE command_list_type;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> command_list;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> command_allocator;

    std::shared_ptr<CommandList> compute_command_list;

    ID3D12RootSignature* root_signature;

    ID3D12PipelineState* pipeline_state;

    std::unique_ptr<UploadBuffer> upload_buffer;

    std::unique_ptr<ResourceStateTracker> resource_state_tracker;

    std::unique_ptr<DynamicDescriptorHeap> dynamic_descriptor_heap[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES];

    ID3D12DescriptorHeap* descriptor_heaps[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES];

    using TrackedObjects = std::vector<Microsoft::WRL::ComPtr<ID3D12Object>>;
    TrackedObjects tracked_objects;
};

export class MakeCommandList : public CommandList
{
public:
    MakeCommandList(Device& device, D3D12_COMMAND_LIST_TYPE type);
    ~MakeCommandList() override;
};

} // namespace gg;
