module;

#define NOMINMAX

#include <cstdint>
#include <mutex>

#include <d3d12.h>

module render;

import :device;
import :descriptor_allocator;
import :descriptor_allocation;
import :descriptor_allocator_page;

namespace gg
{
    DescriptorAllocation DescriptorAllocator::allocate(uint32_t num_descriptors)
    {
        std::lock_guard<std::mutex> lock(this->allocation_mutex);

        DescriptorAllocation allocation;

        auto iter = this->available_heaps.begin();
        while(iter != this->available_heaps.end())
        {
            std::shared_ptr<DescriptorAllocatorPage> allocator_page = this->heap_pool[*iter];

            allocation = allocator_page->allocate(num_descriptors);

            if(allocator_page->numFreeHandles() == 0)
            {
                iter = this->available_heaps.erase(iter);
            }
            else
            {
                ++iter;
            }

            if(allocation.is_valid())
            {
                break;
            }
        }

        if(allocation.is_null())
        {
            this->num_descriptors_per_heap = std::max(this->num_descriptors_per_heap, num_descriptors);
            std::shared_ptr<DescriptorAllocatorPage> new_page = this->createAllocatorPage();

            allocation = new_page->allocate(num_descriptors);
        }

        return allocation;
    }

    void DescriptorAllocator::releaseStaleDescriptors()
    {
        std::lock_guard<std::mutex> lock(this->allocation_mutex);

        for(size_t i = 0; i < this->heap_pool.size(); ++i)
        {
            std::shared_ptr<DescriptorAllocatorPage> page = this->heap_pool[i];

            page->releaseStaleDescriptors();

            if(page->numFreeHandles() > 0)
            {
                this->available_heaps.insert(i);
            }
        }
    }

    DescriptorAllocator::DescriptorAllocator(Device& device, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t num_descriptors_per_heap)
        : device{device}
        , d3d12_heap_type{type}
        , num_descriptors_per_heap{num_descriptors_per_heap}
    {}

    DescriptorAllocator::~DescriptorAllocator() {}

    std::shared_ptr<DescriptorAllocatorPage> DescriptorAllocator::createAllocatorPage()
    {
        std::shared_ptr<DescriptorAllocatorPage> new_page = std::make_shared<MakeDescriptorAllocatorPage>(this->device, this->d3d12_heap_type, this->num_descriptors_per_heap);

        this->heap_pool.emplace_back(new_page);
        this->available_heaps.insert(this->heap_pool.size() - 1);

        return new_page;
    }

    MakeDescriptorAllocator::MakeDescriptorAllocator(Device& device, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t num_descriptors_per_heap)
        : DescriptorAllocator(device, type, num_descriptors_per_heap)
    {}

    MakeDescriptorAllocator::~MakeDescriptorAllocator() {}

} // namespace gg;
