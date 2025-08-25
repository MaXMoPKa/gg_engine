#pragma once

#include <Windows.h>

#include "src/include/exception.hpp"
#include "src/include/keyboard.hpp"
#include "src/include/mouse.hpp"

#include <optional>
#include <memory>

namespace gg
{

namespace gapi
{
    class DX11Graphics;
}

class Window
{
public:
    class WindowException : public Exception
    {
        using Exception::Exception;
    public:
        static std::string translateErrorCode(HRESULT hr) noexcept;
    };
    class HrException : public WindowException
    {
    public:
        HrException(int line, const char* file, HRESULT hr) noexcept;

        const char* what() const noexcept override;
        const char* getType() const noexcept override;
        HRESULT getErrorCode() const noexcept;
        std::string getErrorDescription() const noexcept;

    private:
        HRESULT hr = 0;
    };
    class NoGfxException : public WindowException
    {
    public:
        using WindowException::WindowException;
        const char* getType() const noexcept override;
    };

private:
    class WindowClass
    {
    public:
        static const char* getName() noexcept;
        static HINSTANCE getInstance() noexcept;
    private:
        WindowClass() noexcept;
        ~WindowClass();

        WindowClass( const WindowClass& ) = delete;
        WindowClass& operator=( const WindowClass& ) = delete;
        static constexpr const char* window_class_name = "gg_studio_main";
        static WindowClass window_class;
        HINSTANCE h_instance;
    };
public:
    Window(int width, int height, const char* name);
    ~Window();
    Window( const Window& ) = delete;
    Window& operator=( const Window&) = delete;

    void setTitle(const std::string& title);

    static std::optional<int> processMessages();

    gapi::DX11Graphics& getGraphics();
public:
    Keyboard keyboard;
    Mouse mouse;
private:
    static LRESULT CALLBACK handleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    static LRESULT CALLBACK handleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    LRESULT handleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
private:
    int width;
    int height;
    HWND h_window;
    std::unique_ptr<gapi::DX11Graphics> graphics;
};

#define GGWND_EXCEPT(hr) Window::HrException(__LINE__, __FILE__, hr)
#define GGWND_LAST_EXCEPT() Window::HrException(__LINE__, __FILE__, GetLastError())
#define GGWND_NOGFX_EXCEPT() Window::NoGfxException( __LINE__,__FILE__ )

}
