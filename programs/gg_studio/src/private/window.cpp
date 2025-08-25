#include "src/include/window.h"
#include "src/include/dx11_graphics.hpp"

#include <memory>
#include <minwindef.h>
#include <optional>
#include <sstream>
#include <winbase.h>
#include <winnt.h>

#include "resources/resource.h"

gg::Window::WindowClass gg::Window::WindowClass::window_class;

gg::Window::HrException::HrException(int line, const char* file, HRESULT hr) noexcept
    : WindowException(line, file)
    , hr(hr)
{}

const char* gg::Window::HrException::what() const noexcept
{
    std::ostringstream oss;
    oss << getType() << std::endl
        << "[Error Code] 0x" << std::hex << std::uppercase << getErrorCode()
        << std::dec << " (" << static_cast<unsigned long>(getErrorCode()) << ")" << std::endl
        << "[Description] " << getErrorDescription() << std::endl
        << getOriginString();
    what_buffer = oss.str();
    return what_buffer.c_str();
}

const char* gg::Window::HrException::getType() const noexcept
{
    return "GG Window Exception";
}

std::string gg::Window::WindowException::translateErrorCode(HRESULT hr) noexcept
{
    char* msg_buf = nullptr;
    const DWORD msg_len = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,hr,MAKELANGID( LANG_NEUTRAL,SUBLANG_DEFAULT ),
        reinterpret_cast<LPSTR>(&msg_buf), 0, nullptr);
    if( msg_len == 0 )
    {
    return "Unidentified error code";
    }
    std::string errorString = msg_buf;
    LocalFree( msg_buf );
    return errorString;
}

HRESULT gg::Window::HrException::getErrorCode() const noexcept
{
    return hr;
}

std::string gg::Window::HrException::getErrorDescription() const noexcept
{
    return WindowException::translateErrorCode(hr);
}

const char* gg::Window::NoGfxException::getType() const noexcept
{
    return "GG Window Exception [No Graphics]";
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
    : width(width)
    , height(height)
{
    RECT wr;
    wr.left = 100;
    wr.right = width + wr.left;
    wr.top = 100;
    wr.bottom = height + wr.top;
    if(AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
    {
        throw GGWND_LAST_EXCEPT();
    }

    h_window = CreateWindow(WindowClass::getName(), name, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top, nullptr, nullptr, WindowClass::getInstance(), this);

    if(h_window == nullptr)
    {
        throw GGWND_LAST_EXCEPT();
    }

    ShowWindow(h_window, SW_SHOWDEFAULT);

    graphics = std::make_unique<gapi::DX11Graphics>(h_window);
}

gg::Window::~Window()
{
    DestroyWindow(h_window);
}

void gg::Window::setTitle(const std::string& title)
{
    if(SetWindowText(h_window, title.c_str()) == 0)
    {
        throw GGWND_LAST_EXCEPT();
    }
}

std::optional<int> gg::Window::processMessages()
{
    MSG msg;

    while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if(msg.message == WM_QUIT)
        {
            return msg.wParam;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return {};
}

gg::gapi::DX11Graphics& gg::Window::getGraphics()
{
    if(graphics == nullptr)
    {
        throw GGWND_NOGFX_EXCEPT();
    }
    return *graphics;
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
    case WM_KILLFOCUS:
    {
        keyboard.clearState();
        break;
    }
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    {
        if(!(lParam & 0x40000000) || keyboard.autorepeatIsEnabled())
        {
            keyboard.onKeyPressed(static_cast<unsigned char>(wParam));
        }
        break;
    }
    case WM_KEYUP:
    case WM_SYSKEYUP:
    {
        keyboard.onKeyReleased(static_cast<unsigned char>(wParam));
        break;
    }
    case WM_CHAR:
    {
        keyboard.onChar(static_cast<unsigned char>(wParam));
        break;
    }
    case WM_MOUSEMOVE:
    {
        const POINTS pt = MAKEPOINTS(lParam);

        if(pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
        {
            mouse.onMouseMove(pt.x, pt.y);
            if(!mouse.isInWindow())
            {
                SetCapture(h_window);
                mouse.onMouseEnter(pt.x, pt.y);
            }
        }
        else
        {
            if(wParam & (MK_LBUTTON | MK_RBUTTON))
            {
                mouse.onMouseMove(pt.x, pt.y);
            }
            else
            {
                ReleaseCapture();
                mouse.onMouseLeave(pt.x, pt.y);
            }
        }

        mouse.onMouseMove(pt.x, pt.y);
        break;
    }
    case WM_LBUTTONDOWN:
    {
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.onLeftPressed(pt.x, pt.y);
        break;
    }
    case WM_RBUTTONDOWN:
    {
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.onRightPressed(pt.x, pt.y);
        break;
    }
    case WM_LBUTTONUP:
    {
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.onLeftReleased(pt.x, pt.y);
        break;
    }
    case WM_RBUTTONUP:
    {
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.onRightReleased(pt.x, pt.y);
        break;
    }
    case WM_MOUSEWHEEL:
    {
        const POINTS pt = MAKEPOINTS(lParam);
        const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
        mouse.onWheelDelta(pt.x, pt.y, delta);
        break;
    }
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}
