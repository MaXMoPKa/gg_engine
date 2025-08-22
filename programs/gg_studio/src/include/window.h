#pragma once

#include <Windows.h>

#include "src/include/exception.hpp"

namespace gg
{

class Window
{
public:
    class WindowException : public Exception
    {
    public:
        WindowException(int line, const char* file, HRESULT hr) noexcept;

        const char* what() const noexcept override;
        const char* getType() const noexcept override;
        static std::string translateErrorCode(HRESULT hr) noexcept;
        HRESULT getErrorCode() const noexcept;
        std::string getErrorString() const noexcept;

    private:
        HRESULT hr;
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
private:
    static LRESULT CALLBACK handleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    static LRESULT CALLBACK handleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    LRESULT handleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
private:
    int width;
    int height;
    HWND h_window;
};

#define GGWND_EXCEPT(hr) Window::WindowException(__LINE__, __FILE__, hr);
#define GGWND_LAST_EXCEPT() Window::WindowException(__LINE__, __FILE__, GetLastError());

}
