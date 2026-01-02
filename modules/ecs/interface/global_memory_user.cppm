module;

export module ecs.global_memory_user;

import ecs.memory_manager;

import types.base_types;
import types.string;

namespace gg
{
namespace memory
{

export class GlobalMemoryUser
{
public:
    GlobalMemoryUser();
    virtual ~GlobalMemoryUser() = default;

    const void* allocate(Size mem_size, const String& user = String());
     void free(void* mem);
private:
    MemoryManager* memory_manager;
};

} // namespace memory;
} // namespace gg;
