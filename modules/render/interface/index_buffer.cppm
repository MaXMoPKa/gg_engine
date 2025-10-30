module;

#include <d3d12.h>

#include <wrl.h>

export module render:index_buffer;

import :buffer;

namespace gg
{

class Device;

export class IndexBuffer : public Buffer
{
public:
    [[nodiscard]] inline D3D12_INDEX_BUFFER_VIEW getIndexBufferView() const
    {
        return this->index_buffer_view;
    }

    [[nodiscard]] inline size_t getNumIndices() const
    {
        return this->num_indices;
    }

    [[nodiscard]] inline DXGI_FORMAT getIndexFormat() const
    {
        return this->index_format;
    }
protected:
    IndexBuffer(Device& device, size_t num_indices, DXGI_FORMAT index_format);
    IndexBuffer(Device& device, Microsoft::WRL::ComPtr<ID3D12Resource> resource, size_t num_indices, DXGI_FORMAT index_format);
    virtual ~IndexBuffer() = default;

    void createIndexBufferView();
private:
    size_t num_indices;
    DXGI_FORMAT index_format;
    D3D12_INDEX_BUFFER_VIEW index_buffer_view;
};

} // namespace gg;
