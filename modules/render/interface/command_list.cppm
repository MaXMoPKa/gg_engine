module;

#include <vector>
#include <memory>
#include <cstdint>

#include <d3d12.h>

#include <wrl.h>

export module render:command_list;

namespace gg
{

class Buffer;
class ByteAddressBuffer;
class ConstantBuffer;
class ConstantBufferView;
class Device;
class DynamicDescriptorHeap;
class GenerateMipsPSO;
class IndexBuffer;
class PanoToCubemapPSO;
class PiplineStateObject;
class RenderTarget;
class Resource;
class ResourceStateTracker;
class RootSignature;
class Scene;
class ShaderResourceView;
class StructuredBuffer;
class Texture;
class UnorderedAccessView;
class UploadBuffer;
class VertexBuffer;

export class CommandList : public std::enable_shared_from_this<CommandList>
{
public:

    [[nodiscard]] inline D3D12_COMMAND_LIST_TYPE getCommandListType() const
    {
        return this->d3d12_command_list_type;
    }

    [[nodiscard]] inline Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> getD3D12CommandList() const
    {
        return this->d3d12_command_list;
    }

    [[nodiscard]] inline Device& getDevice() const
    {
        return this->device;
    }

    void transitionBarrier(const std::shared_ptr<Resource>& resource, D3D12_RESOURCE_STATES state_after,
                           UINT subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, bool flush_barriers = false);
    void transitionBarrier(Microsoft::WRL::ComPtr<ID3D12Resource> resource, D3D12_RESOURCE_STATES state_after,
                           UINT subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, bool flush_barriers = false);

    void uavBarrier(const std::shared_ptr<Resource>& resource = nullptr, bool flush_barrier = false);
    void uavBarrier(Microsoft::WRL::ComPtr<ID3D12Resource> resource, bool flush_barrier = false);

    void aliasingBarrier(const std::shared_ptr<Resource>& before_resource = nullptr, const std::shared_ptr<Resource>& after_resource = nullptr, bool flush_barriers = false);
    void aliasingBarrier(Microsoft::WRL::ComPtr<ID3D12Resource> before_resource, Microsoft::WRL::ComPtr<ID3D12Resource> after_resource, bool flush_barriers = false);

    void flushResourceBarriers();

    void copyResource(const std::shared_ptr<Resource>& dst_resource, const std::shared_ptr<Resource>& src_resource);
    void copyResource(Microsoft::WRL::ComPtr<ID3D12Resource> dst_resource, Microsoft::WRL::ComPtr<ID3D12Resource> src_resource);

    void resolveSubresource(const std::shared_ptr<Resource>& dst_resource, const std::shared_ptr<Resource>& src_resource,
                            uint32_t dst_subresource = 0, uint32_t src_subresource = 0);

    [[nodiscard]] std::shared_ptr<VertexBuffer> copyVertexBuffer(size_t num_vertices, size_t vertex_stride, const void* vertex_buffer_data);
    template<typename T>
    [[nodiscard]] std::shared_ptr<VertexBuffer> copyVertexBuffer(const std::vector<T>& vertex_buffer_data)
    {
        return this->copyVertexBuffer(vertex_buffer_data.size(), sizeof(T), vertex_buffer_data.data());
    }

    [[nodiscard]] std::shared_ptr<IndexBuffer> copyIndexBuffer(size_t num_indices, DXGI_FORMAT index_format, const void* index_buffer_data);
    template<typename T>
    [[nodiscard]] std::shared_ptr<IndexBuffer> copyVertexBuffer(const std::vector<T>& index_buffer_data)
    {
        assert(sizeof(T) == 2 || sizeof(T) == 4);
        DXGI_FORMAT index_format = (sizeof(T) == 2) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
        return this->copyIndexBuffer(index_buffer_data.size(), index_format, index_buffer_data.data());
    }

    [[nodiscard]] std::shared_ptr<ConstantBuffer> copyConstantBuffer(size_t buffer_size, const void* buffer_data);
    template<typename T>
    [[nodiscard]] std::shared_ptr<ConstantBuffer> copyConstantBuffer(const T& data)
    {
        return this->copyConstantBuffer(sizeof(T), &data);
    }

    [[nodiscard]] std::shared_ptr<ByteAddressBuffer> copyByteAddressBuffer(size_t buffer_size, const void* buffer_data);
    template<typename T>
    [[nodiscard]] std::shared_ptr<ByteAddressBuffer> copyByteAddressBuffer(const T& data)
    {
        return this->copyByteAddressBuffer(sizeof(T), &data);
    }

    [[nodiscard]] std::shared_ptr<StructuredBuffer> copyStructuredBuffer(size_t num_elements, size_t element_size, const void* buffer_data);
    template<typename T>
    [[nodiscard]] std::shared_ptr<StructuredBuffer> copyStructuredBuffer(const std::vector<T>& buffer_data)
    {
        return this->copyStructuredBuffer(buffer_data.size(), sizeof(T), buffer_data.data());
    }

    void setPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY primitive_topology);

    void clearTexture(const std::shared_ptr<Texture>& texture, const float clear_color[4]);

    void clearDepthStencilTexture(const std::shared_ptr<Texture>& texture, D3D12_CLEAR_FLAGS clear_flags, float depth = 1.0f, uint8_t stencil = 0);

    void generateMips(const std::shared_ptr<Texture>& texture);

    void copySubresourceDataToTexture(const std::shared_ptr<Texture>& texture, uint32_t first_subresource, uint32_t num_subresources, D3D12_SUBRESOURCE_DATA* subresource_data);

    void setGraphicsDynamicConstantBuffer(uint32_t root_parameter_index, size_t size_in_bytes, const void* buffer_data);
    template<typename T>
    void setGraphicsDynamicConstantBuffer(uint32_t root_parameter_index, const T& data)
    {
        this->setGraphicsDynamicConstantBuffer(root_parameter_index, sizeof(T), &data);
    }

    void setGraphics32BitConstants(uint32_t root_parameter_index, uint32_t num_constants, const void* constants);
    template<typename T>
    void setGraphics32BitConstants(uint32_t root_parameter_index, const T& constants)
    {
        static_assert((sizeof(T) % sizeof(uint32_t)) == 0, "Size of type must be a multiple of 4 bytes");
        this->setGraphics32BitConstants(root_parameter_index, sizeof(T) / sizeof(uint32_t), &constants);
    }

    void setCompute32BitConstants(uint32_t root_parameter_index, uint32_t num_constants, const void* constants);
    template<typename T>
    void setCompute32BitConstants(uint32_t root_parameter_index, const T& constants)
    {
        static_assert((sizeof(T) % sizeof(uint32_t)) == 0, "Size of type must be a multiple of 4 bytes");
        this->setCompute32BitConstants(root_parameter_index, sizeof(T) / sizeof(uint32_t), &constants);
    }

    void setVertexBuffers(uint32_t start_slot, const std::vector<std::shared_ptr<VertexBuffer>>& vertex_buffer_views);
    void setVertexBuffer(uint32_t slot, const std::shared_ptr<VertexBuffer>& vertex_buffer_view);

    void setDynamicVertexBuffer(uint32_t slot, size_t num_vertices, size_t vertex_size, const void* vertex_buffer_data);
    template<typename T>
    void setDynamicVertexBuffer(uint32_t slot, const std::vector<T>& vertex_buffer_data)
    {
        this->setDynamicVertexBuffer(slot, vertex_buffer_data.size(), sizeof(T), vertex_buffer_data.data());
    }

    void setIndexBuffer(const std::shared_ptr<IndexBuffer>&  index_buffer);

    void setDynamicIndexBuffer(size_t num_indices, DXGI_FORMAT index_format, const void* index_buffer_data);
    template<typename T>
    void setDynamicIndexBuffer(const std::vector<T>& index_buffer_data)
    {
        static_assert(sizeof(T) == 2 || sizeof(T) == 4);
        DXGI_FORMAT index_format = (sizeof(T) == 2) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
        this->setDynamicIndexBuffer(index_buffer_data.size(), index_format, index_buffer_data.data());
    }

    void setGraphicsDynamicStructuredBuffer(uint32_t slot, size_t num_elements, size_t element_size, const void* buffer_data);
    template<typename T>
    void setGraphicsDynamicStructuredBuffer(uint32_t slot, const std::vector<T>& buffer_data)
    {
        this->setGraphicsDynamicStructuredBuffer(slot, buffer_data.size(), sizeof(T), buffer_data.data());
    }

    void setViewport(const D3D12_VIEWPORT& viewport);
    void setViewports(const std::vector<D3D12_VIEWPORT>& viewports);

    void setScissorRect(const D3D12_RECT& scissor_rect);
    void setScissorRects(const std::vector<D3D12_RECT>& scissor_rects);

    void setPiplineState(const std::shared_ptr<PiplineStateObject>& pipline_state);

    void setGraphicsRootSignature(const std::shared_ptr<RootSignature>& root_signature);
    void setComputeRootSignature(const std::shared_ptr<RootSignature>& root_signature);

    void setConstantBufferView(uint32_t root_parameter_index, const std::shared_ptr<ConstantBuffer>& buffer,
                               D3D12_RESOURCE_STATES state_after = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
                               size_t buffer_offset = 0);

    void setShaderResourceView(uint32_t root_parameter_index, const std::shared_ptr<Buffer>& buffer,
                               D3D12_RESOURCE_STATES state_after = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE |
                                                                   D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
                               size_t buffer_offset = 0);

    void setUnorderedAccessView(uint32_t root_parameter_index, const std::shared_ptr<Buffer>& buffer,
                                D3D12_RESOURCE_STATES state_after = D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
                                size_t buffer_offset = 0);

    void setConstantBufferView(uint32_t root_parameter_index, uint32_t descriptor_offset,
                               const std::shared_ptr<ConstantBufferView>& constant_buffer_view,
                               D3D12_RESOURCE_STATES state_after = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

    void setShaderResourceView(uint32_t root_parameter_index, uint32_t descriptor_offset,
                               const std::shared_ptr<ShaderResourceView>& shader_resource_view,
                               D3D12_RESOURCE_STATES state_after = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE |
                                                                   D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
                               UINT first_subresource = 0,
                               UINT num_subresources = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);

    void setShaderResourceView(uint32_t root_parameter_index, uint32_t descriptor_offset,
                               const std::shared_ptr<Texture>& texture,
                               D3D12_RESOURCE_STATES state_after = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE |
                                                                   D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
                               UINT first_subresource = 0,
                               UINT num_subresources = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);

    void setUnorderedAccessView(uint32_t root_parameter_index, uint32_t descriptor_offset,
                                const std::shared_ptr<UnorderedAccessView>& unordered_access_view,
                                D3D12_RESOURCE_STATES state_after = D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
                                UINT first_subresource = 0,
                                UINT num_subresources = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);

    void setUnorderedAccessView(uint32_t root_parameter_index, uint32_t descriptor_offset,
                                const std::shared_ptr<Texture>& texture, UINT mip,
                                D3D12_RESOURCE_STATES state_after = D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
                                UINT first_subresource = 0,
                                UINT num_subresources = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);

    void setRenderTarget(const RenderTarget& render_target);

    void draw(uint32_t vertex_count, uint32_t instance_count = 1, uint32_t start_vertex = 0, uint32_t start_instance = 0);
    void drawIndexed(uint32_t index_count, uint32_t instance_count = 1, uint32_t start_index = 0, int32_t base_vertex = 0, uint32_t start_instance = 0);

    void dispatch(uint32_t num_groups_x, uint32_t num_groups_y = 1, uint32_t num_groups_z = 1);

protected:
    friend class CommandQueue;
    friend class std::default_delete<CommandList>;

    CommandList(Device& device, D3D12_COMMAND_LIST_TYPE type);
    virtual ~CommandList();

    [[nodiscard]] bool close(const std::shared_ptr<CommandList>& pending_command_list);
    void close();

    void reset();

    void releaseTrackedObjects();

    void setDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heap_type, ID3D12DescriptorHeap* heap);

    [[nodiscard]] inline std::shared_ptr<CommandList> getGenerateMipsCommandList() const
    {
        return this->compute_command_list;
    }
private:
    void trackResource(Microsoft::WRL::ComPtr<ID3D12Object> object);
    void trackResource(const std::shared_ptr<Resource>& resource);

    void generateMipsUAV(const std::shared_ptr<Texture>& texture, bool is_srgb);

    [[nodiscard]] Microsoft::WRL::ComPtr<ID3D12Resource> copyBuffer(size_t buffer_size, const void* buffer_data, D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE);

    void bindDescriptorHeaps();
private:
    Device& device;
    D3D12_COMMAND_LIST_TYPE d3d12_command_list_type;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> d3d12_command_list;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> d3d12_command_allocator;

    std::shared_ptr<CommandList> compute_command_list;

    ID3D12RootSignature* root_signature;

    ID3D12PipelineState* pipeline_state;

    std::unique_ptr<UploadBuffer> upload_buffer;

    std::unique_ptr<ResourceStateTracker> resource_state_tracker;

    std::unique_ptr<DynamicDescriptorHeap> dynamic_descriptor_heap[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES];

    ID3D12DescriptorHeap* d3d12_descriptor_heaps[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES];

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
