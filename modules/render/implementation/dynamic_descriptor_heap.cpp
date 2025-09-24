module;

#include <cstdint>

#include <d3d12.h>

module render;

import :dynamic_descriptor_heap;
import :device;

namespace gg
{
    DynamicDescriptorHeap::DynamicDescriptorHeap(Device& device, D3D12_DESCRIPTOR_HEAP_TYPE heap_type, uint32_t num_descriptors_per_heap)
        : device{device}
        , d3d12_descriptor_heap_type{heap_type}
        , num_descriptors_per_heap{num_descriptors_per_heap}
    {}

    DynamicDescriptorHeap::~DynamicDescriptorHeap() {}

    void DynamicDescriptorHeap::reset()
    {

    }
} // namespace gg;
