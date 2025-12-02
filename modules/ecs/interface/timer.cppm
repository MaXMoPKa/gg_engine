module;

#include <chrono>
#include <ratio>

export module ecs:timer;

import :platform;

namespace gg
{
namespace util
{
export class Timer
{
    using Elapsed = std::chrono::duration<float, std::milli>;
public:
    Timer() = default;
    ~Timer() = default;

    void tick(float ms) {};
    void reset() {};
    [[nodiscard]] inline TimeStamp getTimeStamp() const
    {
        return TimeStamp(this->elapsed.count());
    }
private:
    Elapsed elapsed;
};
} // namespace util;
} // namespace gg;
