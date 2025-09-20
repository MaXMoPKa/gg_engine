module;

#include <cstdint>
#include <mutex>

#include <d3d12.h>

#include <wrl.h>

module render;

import :descriptor_allocator_page;
import :device;
import :command_queue;
import :descriptor_allocation;

namespace gg
{

D3D12_DESCRIPTOR_HEAP_TYPE DescriptorAllocatorPage::getHeapType() const
{
    return this->heap_type;
}

bool DescriptorAllocatorPage::hasSpace(uint32_t num_descriptors) const
{
    return this->free_list_by_size.lower_bound(num_descriptors) != this->free_list_by_size.end();
}

uint32_t DescriptorAllocatorPage::numFreeHandles() const
{
    return this->num_free_handles;
}

DescriptorAllocation DescriptorAllocatorPage::allocate(uint32_t num_descriptors)
{
    std::lock_guard<std::mutex> lock(this->allocation_mutex);

    if(num_descriptors > this->num_free_handles)
    {
        return DescriptorAllocation();
    }

    auto smallest_block_it = this->free_list_by_size.lower_bound(num_descriptors);
    if(smallest_block_it == this->free_list_by_size.end())
    {
        return DescriptorAllocation();
    }

    SizeType block_size = smallest_block_it->first;
    auto offset_it = smallest_block_it->second;

    OffsetType offset = offset_it->first;

    this->free_list_by_size.erase(smallest_block_it);
    this->free_list_by_offset.erase(offset_it);

    OffsetType new_offset = offset + num_descriptors;
    SizeType new_size = block_size - num_descriptors;

    if(new_size > 0)
    {
        this->addNewBlock(new_offset, new_size);
    }

    this->num_free_handles -= num_descriptors;

    D3D12_CPU_DESCRIPTOR_HANDLE handle;
    handle.ptr = static_cast<SIZE_T>(this->base_descriptor.ptr + INT64(offset) * UINT64(this->descriptor_handle_increment_size));
    return DescriptorAllocation(handle, num_descriptors, this->descriptor_handle_increment_size, shared_from_this());
}

void DescriptorAllocatorPage::free(DescriptorAllocation&& descriptor)
{
    uint32_t offset = computeOffset(descriptor.getDescriptorHandle());
    std::lock_guard<std::mutex> lock(this->allocation_mutex);
    this->stale_descriptors.emplace(offset, descriptor.getNumHandles());
}

void DescriptorAllocatorPage::releaseStaleDescriptors()
{
    std::lock_guard<std::mutex> lock(this->allocation_mutex);
    while(!this->stale_descriptors.empty())
    {
        StaleDescriptorInfo& stale_descriptor = this->stale_descriptors.front();

        OffsetType offset = stale_descriptor.offset;
        SizeType num_descriptors = stale_descriptor.size;

        this->freeBlock(offset, num_descriptors);

        this->stale_descriptors.pop();
    }
}

DescriptorAllocatorPage::DescriptorAllocatorPage(Device& device, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t num_descriptors)
    : device{device}
    , heap_type{type}
    , num_descriptors_in_heap{num_descriptors}
{
    Microsoft::WRL::ComPtr<ID3D12Device2> d3d12_device = this->device.getD3D12Device();

    D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};
    heap_desc.Type = this->heap_type;
    heap_desc.NumDescriptors = this->num_descriptors_in_heap;

    throwIfFailed(d3d12_device->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(&this->descriptor_heap)));

    this->base_descriptor = this->descriptor_heap->GetCPUDescriptorHandleForHeapStart();
    this->descriptor_handle_increment_size = d3d12_device->GetDescriptorHandleIncrementSize(this->heap_type);
    this->num_free_handles = this->num_descriptors_in_heap;

    this->addNewBlock(0, this->num_free_handles);
}

uint32_t DescriptorAllocatorPage::computeOffset(D3D12_CPU_DESCRIPTOR_HANDLE handle)
{
    return static_cast<uint32_t>(handle.ptr - this->base_descriptor.ptr) / this->descriptor_handle_increment_size;
}

void DescriptorAllocatorPage::addNewBlock(uint32_t offset, uint32_t num_descriptors)
{
    auto offset_it = this->free_list_by_offset.emplace(offset, num_descriptors);
    auto size_it = this->free_list_by_size.emplace(num_descriptors, offset_it.first);
    offset_it.first->second.free_list_by_size_iterator = size_it;
}

void DescriptorAllocatorPage::freeBlock(uint32_t offset, uint32_t num_descriptors)
{
    auto next_block_it = this->free_list_by_offset.upper_bound(offset);
    auto prev_block_it = next_block_it;

    if(prev_block_it != this->free_list_by_offset.begin())
    {
        --prev_block_it;
    }
    else
    {
        prev_block_it = this->free_list_by_offset.end();
    }

    this->num_free_handles += num_descriptors;

    if(prev_block_it != this->free_list_by_offset.end() && offset == prev_block_it->first + prev_block_it->second.size)
    {
        offset = prev_block_it->first;
        num_descriptors += prev_block_it->second.size;

        this->free_list_by_size.erase(prev_block_it->second.free_list_by_size_iterator);
        this->free_list_by_offset.erase(prev_block_it);
    }

    if(next_block_it != this->free_list_by_offset.end() && offset + num_descriptors == next_block_it->first)
    {
        num_descriptors += next_block_it->second.size;

        this->free_list_by_size.erase(next_block_it->second.free_list_by_size_iterator);
        this->free_list_by_offset.erase(next_block_it);
    }

    this->addNewBlock(offset, num_descriptors);
}

MakeDescriptorAllocatorPage::MakeDescriptorAllocatorPage(Device& device, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t num_descriptors)
    : DescriptorAllocatorPage{device, type, num_descriptors}
{}

MakeDescriptorAllocatorPage::~MakeDescriptorAllocatorPage() {}

} // namespace gg;
