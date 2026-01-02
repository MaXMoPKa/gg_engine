module;

#include <chrono>
#include <ratio>

export module ecs.timer;

import ecs.platform;

import types.base_types;

namespace gg
{
namespace util
{
export class Timer
{
    using Elapsed = std::chrono::duration<F32, std::milli>;
public:
    Timer() = default;
    ~Timer() = default;

    void tick(F32 ms) {};
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
