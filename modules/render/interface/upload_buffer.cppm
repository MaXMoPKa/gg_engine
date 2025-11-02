module;

#include <deque>
#include <memory>

#include <d3d12.h>

#include <wrl.h>

export module render:upload_buffer;

namespace gg
{

class Device;

export class UploadBuffer
{
public:
    struct Allocation
    {
        void* CPU;
        D3D12_GPU_VIRTUAL_ADDRESS GPU;
    };

    [[nodiscard]] inline size_t getPageSize() const
    {
        return this->page_size;
    }

    [[nodiscard]] Allocation allocate(size_t size_in_bytes, size_t alignment);

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
        Page(Device& device, size_t size_in_bytes);
        ~Page();

        bool hasSpace(size_t size_in_bytes, size_t alignment) const;

        Allocation allocate(size_t size_in_bytes, size_t alignment);

        void reset();
    private:
        Device& device;
        Microsoft::WRL::ComPtr<ID3D12Resource> d3d12_resource;

        void* cpu_ptr;
        D3D12_GPU_VIRTUAL_ADDRESS gpu_ptr;

        size_t page_size;

        size_t offset;

    };

    [[nodiscard]] std::shared_ptr<Page> requestPage();

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
