module;

#include <d3d12.h>

#include <wrl.h>

export module render:constant_buffer;

import :buffer;

namespace gg
{

class Device;

export class ConstantBuffer : public Buffer
{
public:
    [[nodiscard]] inline size_t getSizeInBytes() const
    {
        return this->size_in_bytes;
    }
protected:
    ConstantBuffer(Device& device, Microsoft::WRL::ComPtr<ID3D12Resource> resource);
    virtual ~ConstantBuffer();
private:
    size_t size_in_bytes;
};

} // namespace gg;
