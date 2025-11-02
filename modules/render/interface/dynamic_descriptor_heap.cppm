module;

#include <cstdint>
#include <memory>
#include <functional>
#include <queue>

#include <d3d12.h>

#include <wrl.h>

#include <d3dx12.h>

export module render:dynamic_descriptor_heap;

namespace gg
{

class Device;
class CommandList;
class RootSignature;

class DynamicDescriptorHeap
{
public:
    DynamicDescriptorHeap(Device& device, D3D12_DESCRIPTOR_HEAP_TYPE heap_type, uint32_t num_descriptors_per_heap = 1024);
    virtual ~DynamicDescriptorHeap();

    void stageDescriptors(uint32_t root_parameter_index, uint32_t offset, uint32_t num_descriptors,
                          const D3D12_CPU_DESCRIPTOR_HANDLE src_descriptors);

    void stageInlineCBV(uint32_t root_parameter_index, D3D12_GPU_VIRTUAL_ADDRESS buffer_location);

    void stageInlineSRV(uint32_t root_parameter_index, D3D12_GPU_VIRTUAL_ADDRESS buffer_location);

    void stageInlineUAV(uint32_t root_parameter_index, D3D12_GPU_VIRTUAL_ADDRESS buffer_location);

    void commitStagedDescriptorsForDraw(CommandList& command_list);
    void commitStagedDescriptorsForDispatch(CommandList& command_list);

    [[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE copyDescriptor(CommandList& command_list, D3D12_CPU_DESCRIPTOR_HANDLE cpu_descriptor);

    void parseRootSignature(const std::shared_ptr<RootSignature>& root_signature);

    void reset();
private:
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> requestDescriptorHeap();
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> createDescriptorHeap();

    uint32_t computeStaleDescriptorCount() const;

    void commitDescriptorTables(CommandList& command_list,
                                std::function<void(ID3D12GraphicsCommandList*, UINT, D3D12_GPU_DESCRIPTOR_HANDLE)> set_func);

    void commitInlineDescriptors(CommandList& command_list, const D3D12_GPU_VIRTUAL_ADDRESS* buffer_location, uint32_t& bit_mask,
                                 std::function<void(ID3D12GraphicsCommandList*, UINT, D3D12_GPU_VIRTUAL_ADDRESS)> set_func);
private:
    static const uint32_t max_descriptor_tables = 32;
    struct DescriptorTableCache
    {
        DescriptorTableCache()
            : num_descriptors(0)
            , base_descriptor(nullptr)
        {}

        void reset()
        {
            num_descriptors = 0;
            base_descriptor = nullptr;
        }

        uint32_t num_descriptors;
        D3D12_CPU_DESCRIPTOR_HANDLE* base_descriptor;
    };

    Device& device;

    D3D12_DESCRIPTOR_HEAP_TYPE d3d12_descriptor_heap_type;

    uint32_t num_descriptors_per_heap;

    uint32_t descriptor_handle_increment_size;

    std::unique_ptr<D3D12_CPU_DESCRIPTOR_HANDLE[]> descriptor_handle_cache;

    DescriptorTableCache descriptor_table_cache[max_descriptor_tables];

    D3D12_GPU_VIRTUAL_ADDRESS inline_cbv[max_descriptor_tables];
    D3D12_GPU_VIRTUAL_ADDRESS inline_srv[max_descriptor_tables];
    D3D12_GPU_VIRTUAL_ADDRESS inline_uav[max_descriptor_tables];

    uint32_t descriptor_table_bit_mask;

    uint32_t stale_descriptor_table_bit_mask;
    uint32_t stale_cbv_bit_mask;
    uint32_t stale_srv_bit_mask;
    uint32_t stale_uav_bit_mask;

    using DescriptorHeapPool = std::queue<Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>>;

    DescriptorHeapPool descriptor_heap_pool;
    DescriptorHeapPool available_descriptor_heaps;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> current_descriptor_heap;
    CD3DX12_GPU_DESCRIPTOR_HANDLE current_gpu_descriptor_handle;
    CD3DX12_CPU_DESCRIPTOR_HANDLE current_cpu_descriptor_handle;

    uint32_t num_free_handles;
};

} // namespace gg;
