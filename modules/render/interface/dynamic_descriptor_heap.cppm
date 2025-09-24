module;

#include <cstdint>

#include <d3d12.h>

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

    void reset();
private:
    Device& device;

    D3D12_DESCRIPTOR_HEAP_TYPE d3d12_descriptor_heap_type;

    uint32_t num_descriptors_per_heap;
};

} // namespace gg;
