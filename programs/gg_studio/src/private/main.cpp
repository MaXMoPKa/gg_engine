#include "src/include/exception.hpp"
#include "src/include/window.h"
#include <minwindef.h>
#include <winuser.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    try
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
    catch(const gg::Exception& e)
    {
        MessageBox(nullptr, e.what(), e.getType(), MB_OK | MB_ICONEXCLAMATION);
    }
    catch(const std::exception& e)
    {
        MessageBox(nullptr, e.what(), "Standart Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    catch(...)
    {
        MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    return -1;
}

int main()
{
    return WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOWDEFAULT);
}
