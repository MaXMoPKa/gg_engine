module;

#include <queue>
#include <mutex>

export module render:thread_save_queue;

export namespace gg
{

template<typename T>
class ThreadSaveQueue
{
public:
    ThreadSaveQueue() = default;
    ThreadSaveQueue(const ThreadSaveQueue& copy)
    {
        std::lock_guard<std::mutex> lock(copy.mutex);
        this->queue = copy.queue;
    }

    void push(T value)
    {
        std::lock_guard<std::mutex> lock(this->mutex);
        this->queue.push(std::move(value));
    }

    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> lock(this->mutex);
        if(this->queue.empty())
        {
            return false;
        }

        value = this->queue.front();
        this->queue.pop();

        return true;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(this->mutex);
        return this->queue.empty();
    }

    size_t size() const
    {
        std::lock_guard<std::mutex> lock(this->mutex);
        return this->queue.size();
    }

private:
    std::queue<T> queue;
    mutable std::mutex mutex;
};

} // namespace gg;
