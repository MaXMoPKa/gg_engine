#include "src/include/app.hpp"
#include "src/include/window.h"
#include "src/include/dx11_graphics.hpp"

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
    const float c = std::sin(timer.peek()) / 2.0f + 0.5f;
    window.getGraphics().clearBuffer(c, c, 1.0f);
    window.getGraphics().drawTestTriangle();
    window.getGraphics().endFrame();
}
