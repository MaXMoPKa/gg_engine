module;

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <utility>

module ecs.memory_manager;

namespace gg
{
namespace memory
{
MemoryManager::MemoryManager()
{
    this->global_memory = std::malloc(MemoryManager::MEMORY_CAPACITY);
    assert(this->global_memory != nullptr && "Failed to allocate global memory!");

    this->memory_allocator = new StackAllocator(MemoryManager::MEMORY_CAPACITY, this->global_memory);
    assert(this->memory_allocator != nullptr && "Failed to create memory allocator!");

    this->pending_memory.clear();
    this->freed_memory.clear();
}

MemoryManager::~MemoryManager()
{
    this->memory_allocator->clear();
    delete this->memory_allocator;
    this->memory_allocator = nullptr;

    this->free(this->global_memory);
    this->global_memory = nullptr;
}

void* MemoryManager::allocate(Size memory_size, const String& user)
{
    void* memory_ptr = this->memory_allocator->allocate(memory_size, alignof(U8));
    this->pending_memory.push_back(std::make_pair(user, memory_ptr));
    return memory_ptr;
}

void MemoryManager::free(void* memory_ptr)
{
    if(memory_ptr == this->pending_memory.back().second)
    {
        this->memory_allocator->free(memory_ptr);
        this->pending_memory.pop_back();

        Bool check = true;
        while(check)
        {
            check = false;
            const auto& it = std::find_if(this->freed_memory.begin(), this->freed_memory.end(), [&](const void* pointer){return pointer == this->pending_memory.back().second;});

            if(it != this->freed_memory.end())
            {
                this->memory_allocator->free(memory_ptr);
                this->pending_memory.pop_back();
                this->freed_memory.remove(*it);

                check = true;
            }
        }
    }
    else
    {
        this->freed_memory.push_back(memory_ptr);
    }    
}

void MemoryManager::check_memory_leaks()
{
    assert(!(this->freed_memory.size() > 0 && this->pending_memory.size() == 0) && "Implementation failure");
    if(this->pending_memory.size() > 0)
    {
        //LogFatal("!!! M E M O R Y   L E A K   D E T E C T E D !!!");

        for(const auto& i : this->pending_memory)
        {
            Bool is_freed = false;

            is_freed = std::any_of(this->freed_memory.begin(), this->freed_memory.end(), [&i](const void* pointer)
            {
                if(pointer == i.second)
                {
                    return true;
                }

                return false;
            });

            if(!is_freed)
            {
                // LogFatal("\'%s\' memory user didn't release allocated memory %p!", i.first.c_str(), i.second);
            }
        }
    }
    else
    {
        // LogInfo("No memory leaks detected.");
    }
}
    
} // namespace memory;
} // namespace gg;
