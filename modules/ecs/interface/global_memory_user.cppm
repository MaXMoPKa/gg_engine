module;

#include <string>

export module ecs:global_memory_user;

namespace gg
{
namespace memory
{

class MemoryManager;

export class GlobalMemoryUser
{
public:
    GlobalMemoryUser();
    virtual ~GlobalMemoryUser() = default;

    const void* allocate(std::size_t mem_size, const std::string& user = std::string());
     void free(void* mem);
private:
};

} // namespace memory;
} // namespace gg;
