#include <memory>
#include <windows.h>

import core.windows_window;
import core.window_descriptor;
import core.vector;

int main()
{

    gg::WindowDescriptor descriptor{gg::i16vec2{1920, 1080}, "GG Studio"};
    std::shared_ptr<gg::WindowsWindow> window = std::make_shared<gg::WindowsWindow>(descriptor);
    window->create();
    window->open();

    if(!window->isOpen())
    {
        return 1;
    }

    MSG msg = {0};

    while(msg.message != WM_QUIT)
    {
        if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }


    return static_cast<int>(msg.wParam);
}
