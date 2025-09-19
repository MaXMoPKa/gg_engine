module;

#include <queue>
#include <mutex>

export module gg.render.thread_save_queue;

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
        queue = copy.queue;
    }

    void push(T value)
    {
        std::lock_guard<std::mutex> lock(mutex);
        queue.push(std::move(value));
    }

    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> lock(mutex);
        if(queue.empty())
        {
            return false;
        }

        value = queue.front();
        queue.pop();

        return true;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(mutex);
        return queue.empty();
    }

    size_t size() const
    {
        std::lock_guard<std::mutex> lock(mutex);
        return queue.size();
    }

private:
    std::queue<T> queue;
    mutable std::mutex mutex;
};

} // namespace gg;
