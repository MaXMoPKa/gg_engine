module;

#include <memory>

module render;

import :upload_buffer;
import :device;

namespace gg
{
    void UploadBuffer::reset()
    {
        this->current_page = nullptr;
        this->available_pages = this->page_pool;

        for(std::shared_ptr<Page> page : this->available_pages)
        {
            page->reset();
        }
    }

    UploadBuffer::UploadBuffer(Device& device, size_t page_size)
        : device{device}
        , page_size{page_size}
    {}

    UploadBuffer::~UploadBuffer() {}

    void UploadBuffer::Page::reset()
    {
        this->offset = 0;
    }

    MakeUploadBuffer::MakeUploadBuffer(Device& device, size_t page_size)
        : UploadBuffer(device, page_size)
    {}

    MakeUploadBuffer::~MakeUploadBuffer() {};
} // namespace gg;
