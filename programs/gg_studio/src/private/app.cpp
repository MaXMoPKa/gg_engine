#include "src/include/app.hpp"
#include "src/include/window.h"

#include <winuser.h>

gg::App::App()
    : window(800, 600, "GG Studio")
{}

int gg::App::go()
{
    while(true)
    {
        if(const auto ecode = Window::processMessages())
        {
            return *ecode;
        }
        doFrame();
    }
}

void gg::App::doFrame()
{
    const float t = timer.peek();
}
