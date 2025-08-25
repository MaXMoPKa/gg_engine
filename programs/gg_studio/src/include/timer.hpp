#pragma once

#include <chrono>
namespace gg
{

class Timer
{
public:
    Timer();

    float mark();
    float peek() const;
private:
    std::chrono::steady_clock::time_point last;
};

}
