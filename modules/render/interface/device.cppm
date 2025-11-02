module;

#include <memory>
#include <string>

#include <d3d12.h>

#include <wrl.h>

export module render:device;

import gg.render.helpers;

namespace gg
{

class Adapter;
class ByteAddressBuffer;
class CommandQueue;
class CommandList;
class ConstantBuffer;
class ConstantBufferView;
class DescriptorAllocator;
class DescriptorAllocation;
class IndexBuffer;
class PipelineStateObject;
class RenderTarget;
class Resource;
class RootSignature;
class Scene;
class ShaderResourceView;
class StructuredBuffer;
class SwapChain;
class Texture;
class UnorderedAccessView;
class VertexBuffer;

export class Device
{
public:
    static void enableDebugLayer();

    static void reportLiveObjects();

    [[nodiscard]] static std::shared_ptr<Device> create(std::shared_ptr<Adapter> adapter = nullptr);
public:
    std::string getDescription() const;

    [[nodiscard]] DescriptorAllocation allocateDescriptors(D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t num_descriptors = 1);

    [[nodiscard]] inline UINT getDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE type) const
    {
        return this->d3d12_device->GetDescriptorHandleIncrementSize(type);
    }

    [[nodiscard]] std::shared_ptr<SwapChain> createSwapChain(HWND hwnd, DXGI_FORMAT back_buffer_format = DXGI_FORMAT_R10G10B10A2_UNORM);

    [[nodiscard]] std::shared_ptr<ConstantBuffer> createConstantBuffer(Microsoft::WRL::ComPtr<ID3D12Resource> resource);

    [[nodiscard]] std::shared_ptr<ByteAddressBuffer> createByteAddressBuffer(size_t buffer_size);
    [[nodiscard]] std::shared_ptr<ByteAddressBuffer> createByteAddressBuffer(Microsoft::WRL::ComPtr<ID3D12Resource> resource);

    [[nodiscard]] std::shared_ptr<StructuredBuffer> createStructuredBuffer(size_t num_elements, size_t element_size);
    [[nodiscard]] std::shared_ptr<StructuredBuffer> createStructuredBuffer(Microsoft::WRL::ComPtr<ID3D12Resource> resource, size_t num_elements, size_t element_size);

    [[nodiscard]] std::shared_ptr<Texture> createTexture(const D3D12_RESOURCE_DESC& resource_desc, const D3D12_CLEAR_VALUE* clear_value = nullptr);
    [[nodiscard]] std::shared_ptr<Texture> createTexture(Microsoft::WRL::ComPtr<ID3D12Resource> resource, const D3D12_CLEAR_VALUE* clear_value = nullptr);

    [[nodiscard]] std::shared_ptr<IndexBuffer> createIndexBuffer(size_t num_indices, DXGI_FORMAT index_format);
    [[nodiscard]] std::shared_ptr<IndexBuffer> createIndexBuffer(Microsoft::WRL::ComPtr<ID3D12Resource> resource, size_t num_vertices, size_t vertex_stride);

    [[nodiscard]] std::shared_ptr<RootSignature> createRootSignature(const D3D12_ROOT_SIGNATURE_DESC1& root_signature_desc);

    template<class PipelineStateStream>
    [[nodiscard]] std::shared_ptr<PipelineStateStream> createPiplineStateObject(PipelineStateStream& pipeline_state_stream)
    {
        D3D12_PIPELINE_STATE_STREAM_DESC pipeline_state_stream_desc = {sizeof(PipelineStateStream), &pipeline_state_stream};
        return createPiplineStateObject(pipeline_state_stream_desc);
    }

    [[nodiscard]] std::shared_ptr<ConstantBufferView> createConstantBufferView(const std::shared_ptr<ConstantBuffer>& constant_buffer,
                                                                               size_t offset = 0);

    [[nodiscard]] std::shared_ptr<ShaderResourceView> createShaderResourceView(const std::shared_ptr<Resource>& resource,
                                                                               const D3D12_SHADER_RESOURCE_VIEW_DESC* shader_resource_view = nullptr);
    [[nodiscard]] std::shared_ptr<ShaderResourceView> createUnorderedAccessView(const std::shared_ptr<Resource>& resource,
                                                                                const std::shared_ptr<Resource>& counter_resource = nullptr,
                                                                                const D3D12_UNORDERED_ACCESS_VIEW_DESC* unordered_access_view = nullptr);

    void flush();

    void releaseStaleDescriptors();

    [[nodiscard]] inline std::shared_ptr<Adapter> getAdapter() const
    {
        return this->adapter;
    }

    [[nodiscard]] CommandQueue& getCommandQueue(D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT);

    [[nodiscard]] inline Microsoft::WRL::ComPtr<ID3D12Device2> getD3D12Device() const
    {
        return this->d3d12_device;
    }

    [[nodiscard]] inline D3D_ROOT_SIGNATURE_VERSION getHighestRootSignatureVersion() const
    {
        return this->highest_root_signature_version;
    }

    [[nodiscard]] DXGI_SAMPLE_DESC getMultisampleQualityLevels(DXGI_FORMAT format, UINT num_samples = D3D12_MAX_MULTISAMPLE_SAMPLE_COUNT,
                                                               D3D12_MULTISAMPLE_QUALITY_LEVEL_FLAGS flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE) const;

protected:
    Device(std::shared_ptr<Adapter> adapter);
    virtual ~Device();

    [[nodiscard]] std::shared_ptr<PipelineStateObject> createPiplineStateObject(const D3D12_PIPELINE_STATE_STREAM_DESC& pipline_state_stream_desc);

private:
    Microsoft::WRL::ComPtr<ID3D12Device2> d3d12_device;

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
