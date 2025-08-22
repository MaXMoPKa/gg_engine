#include "src/include/window.h"
#include <minwindef.h>
#include <sstream>
#include <winbase.h>
#include <winnt.h>
#include <winuser.h>

#include "resources/resource.h"

gg::Window::WindowClass gg::Window::WindowClass::window_class;

gg::Window::WindowException::WindowException(int line, const char* file, HRESULT hr) noexcept
    : Exception(line, file)
    , hr(hr)
{}

const char* gg::Window::WindowException::what() const noexcept
{
    std::ostringstream oss;
    oss << getType() << std::endl
        << "[Error Code] " << getErrorCode() << std::endl
        << "[Description] " << getErrorString() << std::endl;
    what_buffer = oss.str();
    return what_buffer.c_str();
}

const char* gg::Window::WindowException::getType() const noexcept
{
    return "GG Window Exception";
}

std::string gg::Window::WindowException::translateErrorCode(HRESULT hr) noexcept
{
    char* p_msg_buffer = nullptr;
    DWORD n_msg_len = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                    nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&p_msg_buffer), 0, nullptr);
    if(n_msg_len == 0)
    {
        return "Unidentified error code";
    }

    std::string error_string = p_msg_buffer;
    LocalFree(p_msg_buffer);
    return error_string;
}

HRESULT gg::Window::WindowException::getErrorCode() const noexcept
{
    return hr;
}

std::string gg::Window::WindowException::getErrorString() const noexcept
{
    return translateErrorCode(hr);
}

gg::Window::WindowClass::WindowClass() noexcept
    : h_instance(GetModuleHandle(nullptr))
{
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = handleMsgSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = getInstance();
    wc.hIcon = static_cast<HICON>(LoadImage(h_instance, MAKEINTRESOURCE(IDI_GG_STUDIO), IMAGE_ICON, 32, 32, 0));
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = getName();
    wc.hIconSm = static_cast<HICON>(LoadImage(h_instance, MAKEINTRESOURCE(IDI_GG_STUDIO), IMAGE_ICON, 16, 16, 0));
    RegisterClassEx(&wc);
}

gg::Window::WindowClass::~WindowClass()
{
    UnregisterClass(window_class_name, getInstance());
}

const char* gg::Window::WindowClass::getName() noexcept
{
    return window_class_name;
}

HINSTANCE gg::Window::WindowClass::getInstance() noexcept
{
    return window_class.h_instance;
}

gg::Window::Window(int width, int height, const char* name)
{
    RECT wr;
    wr.left = 100;
    wr.right = width + wr.left;
    wr.top = 100;
    wr.bottom = height + wr.top;
    if(FAILED(AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE)))
    {
        throw GGWND_LAST_EXCEPT();
    }

    h_window = CreateWindow(WindowClass::getName(), name, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top, nullptr, nullptr, WindowClass::getInstance(), this);

    if(h_window == nullptr)
    {
        throw GGWND_LAST_EXCEPT();
    }

    ShowWindow(h_window, SW_SHOWDEFAULT);
}

gg::Window::~Window()
{
    DestroyWindow(h_window);
}

LRESULT WINAPI gg::Window::handleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    if(msg == WM_NCCREATE)
    {
       const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
       Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

       SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

       SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::handleMsgThunk));

       return pWnd->handleMsg(hWnd, msg, wParam, lParam);
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI gg::Window::handleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    return pWnd->handleMsg(hWnd, msg, wParam, lParam);
}

LRESULT gg::Window::handleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    switch(msg)
    {
    case WM_CLOSE:
    {
        PostQuitMessage(0);
        return 0;
    }
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}
