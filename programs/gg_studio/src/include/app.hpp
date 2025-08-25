#pragma once

#include "src/include/window.h"
#include "src/include/timer.hpp"

namespace gg
{

class App
{
public:
    App();

    int go();
private:
    void doFrame();
private:
    Window window;
    Timer timer;
};

}
