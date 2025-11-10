module;

#include <cstdint>
#include <cstddef>
#include <string>
#include <list>

export module ecs:memory_chunk_allocator;

import :global_memory_user;
import :pool_allocator;

namespace gg
{
namespace memory
{

export template <class T, std::size_t MAX_CHUNK_OBJECTS>
class MemoryChunkAllocator : protected GlobalMemoryUser
{
private:
    static const std::size_t MAX_OBJECTS = MAX_CHUNK_OBJECTS;
    static const std::size_t ALLOCATE_SIZE = (sizeof(T) + alignof(T)) * MAX_OBJECTS;
    std::string allocator_tag;
public:
    using Allocator = allocator::PoolAllocator;
    using ObjectList = std::list<T*>;

    class MemoryChunk
    {
    public:
        Allocator* allocator;
        ObjectList objects;

        uintptr_t chunk_start;
        uintptr_t chunk_end;

        MemoryChunk(Allocator* allocator)
            : allocator(allocator)
        {
            this->chunk_start = reinterpret_cast<uintptr_t>(allocator->getMemoryAddress());
            this->chunk_end = this->chunk_start + ALLOCATE_SIZE;
            this->objects.clear();
        }
    };

    using MemoryChunks = std::list<MemoryChunk*>;

    class iterator : public std::iterator<std::forward_iterator_tag, T>
    {
    private:
        typename MemoryChunks::iterator current_chunk;
        typename MemoryChunks::iterator end;
        typename ObjectList::iterator current_object;
    public:
        iterator(typename MemoryChunks::iterator begin, typename MemoryChunks::iterator end)
            : current_chunk(begin)
            , end(end)
        {
            if (begin != end)
            {
                assert((*this->current_chunk) != nullptr);
                this->current_object = (*this->current_chunk)->objects.begin();
            }
            else
            {
                this->current_object = (*std::prev(this->end))->objects.end();
            }
        }

        inline iterator& operator++()
        {
            this->current_object++;

            if (this->current_object == (*this->current_chunk)->objects.end())
            {
                this->current_chunk++;

                if (this->current_chunk != this->end)
                {
                    assert((*this->current_chunk) != nullptr);
                    this->current_object = (*this->current_chunk)->objects.begin();
                }
            }

            return *this;
        }

        inline T& operator*() const { return *this->current_object; }
        inline T* operator->() const { return *this->current_object; }

        inline bool operator==(iterator& other)
        {
            return ((this->current_chunk == other.current_chunk) && (this->current_object == other.current_object));
        }
        inline bool operator!=(iterator& other)
        {
            return ((this->current_chunk != other.current_chunk) && (this->current_object != other.current_object));
        }
    };

    protected:
        MemoryChunks chunks;

    public:
        MemoryChunkAllocator(const std::string& allocator_tag = std::string())
            : allocator_tag(allocator_tag)
        {
            Allocator* allocator = new Allocator(ALLOCATE_SIZE, Allocate(ALLOCATE_SIZE, allocator_tag), sizeof(T), alignof(T));
            this->chunks.push_back(new MemoryChunk(allocator));
        }

        virtual ~MemoryChunkAllocator()
        {
            for(auto chunk : this->chunks)
            {
                for(auto obj : chunk->objects)
                {
                    ((T*)obj)->~T();
                }

                chunk->objects.clear();

                free((void*)chunk->allocator->getMemoryAddress());
                delete chunk->allocator;
                chunk->allocator = nullptr;

                delete chunk;
                chunk = nullptr;
            }
        }

        void* createObject()
        {
            void* slot = nullptr;

            for(auto chunk : this->chunks)
            {
                if(chunk->objects.size() > MAX_OBJECTS)
                {
                    continue;
                }

                slot = chunk->allocator->allocate(sizeof(T), alignof(T));
                if(slot != nullptr)
                {
                    chunk->objects.push_back((T*)slot);
                    break;
                }
            }

            if(slot == nullptr)
            {
                Allocator* allocator = new Allocator(ALLOCATE_SIZE, Allocate(ALLOCATE_SIZE, this->allocator_tag), sizeof(T), alignof(T));
                MemoryChunk* new_chunk = new MemoryChunk(allocator);

                this->chunks.push_front(new_chunk);

                slot = new_chunk->allocator->allocate(sizeof(T), alignof(T));

                assert(slot != nullptr && "Unable to create new object. Out of memory?!");
                new_chunk->objects.clear();
                new_chunk->objects.push_back((T*)slot);
            }

            return slot;
        }

        void destroyObject(void* object)
        {
            uintptr_t adr = reinterpret_cast<uintptr_t>(object);

            for (auto chunk : this->chunks)
            {
                if (chunk->chunk_start <= adr && adr < chunk->chunk_end)
                {
                    chunk->objects.remove((T*)object);
                    chunk->allocator->free(object);
                    return;
                }
            }

            assert(false && "Failed to delete object. Memory corruption?!");
        }

        inline iterator begin() { return iterator(this->chunks.begin(), this->chunks.end()); }
        inline iterator end() { return iterator(this->chunks.end(), this->chunks.end()); }
};

} // namespace memory;
} // namespace gg;
