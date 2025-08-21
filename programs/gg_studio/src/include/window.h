#pragma once

#include <Windows.h>

namespace gg
{

class Window
{
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
    Window(int width, int height, const char* name) noexcept;
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

}
