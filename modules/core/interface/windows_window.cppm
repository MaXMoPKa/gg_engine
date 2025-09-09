module;

#include <windows.h>

export module core.windows_window;

import core.base_window;
import core.window_descriptor;

export namespace gg
{

    class WindowsWindow final : public BaseWindow
    {
    public:
        WindowsWindow(const WindowDescriptor& descriptor)
            : BaseWindow(descriptor)
        {}

        ~WindowsWindow() override
        {
            close();
        }

        WindowsWindow(const WindowsWindow&) = delete;
        WindowsWindow& operator=(WindowsWindow&) = delete;

        WindowsWindow(WindowsWindow&&) = delete;
        WindowsWindow& operator=(WindowsWindow&&) = delete;

    public:
        void create()
        {
            WNDCLASS window_class = {};
            window_class.lpfnWndProc = WindowProc;
            window_class.hInstance = GetModuleHandle(nullptr);
            window_class.lpszClassName = "GG_ENGINE_WINDOW_CLASS";

            RegisterClass(&window_class);

            window_handle = CreateWindowEx(0,
                                           "GG_ENGINE_WINDOW_CLASS",
                                           descriptor.title.c_str(),
                                           WS_OVERLAPPEDWINDOW,
                                           CW_USEDEFAULT,
                                           CW_USEDEFAULT,
                                           descriptor.size.x,
                                           descriptor.size.y,
                                           nullptr,
                                           nullptr,
                                           window_class.hInstance,
                                           this);
        }

        void close()
        {
            if(window_handle)
            {
                DestroyWindow(window_handle);
                window_handle = nullptr;
            }
        }

        void open()
        {
            if(!window_handle)
            {
                create();
            }

            ShowWindow(window_handle, SW_SHOW);
            UpdateWindow(window_handle);
        }

        bool isOpen()
        {
            return IsWindow(window_handle);
        }

    private:
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            switch (uMsg)
            {
                case WM_DESTROY:
                {
                    PostQuitMessage(0);
                    return 0;
                }
                case WM_PAINT:
                {
                    ValidateRect(hwnd, nullptr);
                    return 0;
                }
                case WM_KEYUP:
                {
                    if(wParam == VK_ESCAPE)
                    {
                        PostQuitMessage(0);
                    }
                    return 0;
                }
                default:
                {
                    return DefWindowProc(hwnd, uMsg, wParam, lParam);
                }
            }
        }


    private:
        HWND window_handle;
    };

} // namespace gg;
