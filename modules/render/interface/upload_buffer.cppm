module;

#include <deque>
#include <memory>

export module render:upload_buffer;

namespace gg
{

class Device;

export class UploadBuffer
{
public:
    void reset();
protected:
    friend class std::default_delete<UploadBuffer>;

    explicit UploadBuffer(Device& device, size_t page_size = 2 * 1024 * 1024);
    virtual ~UploadBuffer();
private:
    Device& device;

    struct Page
    {
    public:
        //Page(Device& device, size_t size_in_bytes);
        //~Page();

        //bool has_space(size_t size_in_bytes, size_t alignment) const;

        //Allocation allocate(size_t size_in_bytes, size_t alignment);

        void reset();

    private:
        size_t offset;

    };

    using PagePool = std::deque<std::shared_ptr<Page>>;

    PagePool page_pool;
    PagePool available_pages;

    std::shared_ptr<Page> current_page;

    size_t page_size;
};

export class MakeUploadBuffer : public UploadBuffer
{
public:
    MakeUploadBuffer(Device& device, size_t page_size = 2 * 1024 * 1024);
    ~MakeUploadBuffer() override;
};

} // namespace gg;
