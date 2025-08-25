#include "src/include/timer.hpp"
#include <chrono>

gg::Timer::Timer()
{
    last = std::chrono::steady_clock::now();
}

float gg::Timer::mark()
{
   const auto old = last;
   last = std::chrono::steady_clock::now();
   const std::chrono::duration<float> frame_time = last - old;
   return frame_time.count();
}

float gg::Timer::peek() const
{
  return std::chrono::duration<float>(std::chrono::steady_clock::now() - last).count();
}
