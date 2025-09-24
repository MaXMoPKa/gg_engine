module;

#include <memory>
#include <cassert>

#include <d3d12.h>

module render;

import :descriptor_allocation;

namespace gg
{
    DescriptorAllocation::DescriptorAllocation()
        : d3d12_descriptor{0}
        , num_handles{0}
        , descriptor_size{0}
        , page{nullptr}
    {}

    DescriptorAllocation::DescriptorAllocation(D3D12_CPU_DESCRIPTOR_HANDLE descriptor, uint32_t num_handles, uint32_t descriptor_size, std::shared_ptr<DescriptorAllocatorPage> page)
        : d3d12_descriptor{descriptor}
        , num_handles{num_handles}
        , descriptor_size{descriptor_size}
        , page{page}
    {}

    DescriptorAllocation::~DescriptorAllocation()
    {
        this->free();
    }

    DescriptorAllocation::DescriptorAllocation(DescriptorAllocation&& other) noexcept
        : d3d12_descriptor{other.d3d12_descriptor}
        , num_handles{other.num_handles}
        , descriptor_size{other.descriptor_size}
        , page{std::move(other.page)}
    {
        other.d3d12_descriptor.ptr = 0;
        other.num_handles = 0;
        other.descriptor_size = 0;
    }

    DescriptorAllocation& DescriptorAllocation::operator=(DescriptorAllocation&& other) noexcept
    {
        this->free();

        this->d3d12_descriptor = other.d3d12_descriptor;
        this->num_handles = other.num_handles;
        this->descriptor_size = other.descriptor_size;
        this->page = std::move(other.page);

        other.d3d12_descriptor.ptr = 0;
        other.num_handles = 0;
        other.descriptor_size = 0;

        return *this;
    }

    bool DescriptorAllocation::is_null() const
    {
        return this->d3d12_descriptor.ptr == 0;
    }

    D3D12_CPU_DESCRIPTOR_HANDLE DescriptorAllocation::getDescriptorHandle(uint32_t offset) const
    {
        assert(offset < this->num_handles);
        return {this->d3d12_descriptor.ptr + (this->descriptor_size * offset)};
    }

    uint32_t DescriptorAllocation::getNumHandles() const
    {
        return this->num_handles;
    }

    std::shared_ptr<DescriptorAllocatorPage> DescriptorAllocation::getDescriptorAllocatorPage() const
    {
        return this->page;
    }

    void DescriptorAllocation::free()
    {
        if(this->is_valid() && this->page)
        {
            this->page->free(std::move(*this));

            this->d3d12_descriptor.ptr = 0;
            this->num_handles = 0;
            this->descriptor_size = 0;
            this->page.reset();
        }
    }
} // namespace gg;
