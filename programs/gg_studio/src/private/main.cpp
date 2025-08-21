#include "src/include/window.h"
#include <minwindef.h>
#include <winuser.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    gg::Window wnd(800, 300, "GG Studio");

    MSG msg;
    BOOL gResult;
    while( (gResult = GetMessage(&msg, nullptr, 0, 0)) > 0 )
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if( gResult == -1 )
    {
        return -1;
    }

    return msg.wParam;
}

int main()
{
    return WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOWDEFAULT);
}
