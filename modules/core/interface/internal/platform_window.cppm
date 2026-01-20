module;

#include <cassert>

#if defined(_WIN32)
    #include <windows.h>
#elif defined(__linux__)
    #include <X11/Xlib.h>
#endif

module core.window_manager:platform;

import core.window_descriptor;
import core.window_handle;

import types.base_types;
import types.unordered_map;
import types.string;

namespace gg
{
    struct PlatformWindowData
    {
    #if defined(_WIN32)
        HWND hwnd = nullptr;
    #elif defined(__linux__)
        Display* display = nullptr;
        ::Window x11_window = 0;
    #endif

        Bool should_close = false;
        String title;
    }; // struct PlatformWindowData;

    class WindowManagerImpl
    {
        public:
            static WindowManagerImpl& get()
            {
                static WindowManagerImpl instance;
                return instance;
            }

            WindowHandle create(const WindowDescriptor& descriptor);
            void destroy(WindowHandle handle);
            
            void setTitle(WindowHandle handle, const String& new_title);
            void setSize(WindowHandle handle, U32 new_width, U32 new_height);
            void show(WindowHandle handle);
            void minimize(WindowHandle handle);

            Bool shouldClose(WindowHandle handle) const;
            void processEvents(WindowHandle handle);

        #if defined(_WIN32)
            HWND getHWND(WindowHandle handle) const;
        #elif defined(__linux__)
            struct X11Pair
            {
                Display* display;
                ::Window window;
            }; // struct X11Pair;
            X11Pair getX11(WindowHandle handle) const;
        #endif

            Bool contains(WindowHandle handle) const
            {
                return windows.find(handle.id) != windows.end();
            }
        private:
        #if defined(_WIN32)
            friend LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
        #endif
            
            WindowManagerImpl() = default;
            UnorderedMap<U64, PlatformWindowData> windows;
            U64 next_id = 1u;
    }; // class WindowManagerImpl;

#if defined(_WIN32)
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
    {
        WindowManagerImpl& wm = WindowManagerImpl::get();

        for(auto& [id, data] : wm.windows)
        {
            if(data.hwnd == hwnd)
            {
                if(msg == WM_CLOSE || msg == WM_DESTROY)
                {
                    data.should_close = true;
                    return 0;
                }
                break;
            }
        }

        return DefWindowProc(hwnd, msg, wp, lp);
    }

    WindowHandle WindowManagerImpl::create(const WindowDescriptor& descriptor)
    {
        static Bool class_registered = false;
        if(!class_registered)
        {
            WNDCLASSEX wc = {};
            wc.cbSize = sizeof(wc);
            wc.lpfnWndProc = WndProc;
            wc.hInstance = GetModuleHandle(nullptr);
            wc.lpszClassName = "SandboxWindowClass";
            wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
            RegisterClassEx(&wc);
            class_registered = true;
        }
    
        HWND hwnd = CreateWindowEx(0,
                                   "SandboxWindowClass",
                                   descriptor.title.c_str(),
                                   WS_OVERLAPPEDWINDOW,
                                   CW_USEDEFAULT,
                                   CW_USEDEFAULT,
                                   static_cast<int>(descriptor.width),
                                   static_cast<int>(descriptor.height),
                                   nullptr,
                                   nullptr,
                                   GetModuleHandle(nullptr),
                                   nullptr);

        if(!hwnd)
        {
            return {};
        }

        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);

        U64 id = next_id++;
        windows[id] = PlatformWindowData{ .hwnd = hwnd, .should_close = false, .title = descriptor.title };

        return WindowHandle{.id = id};
    }

    void WindowManagerImpl::destroy(WindowHandle handle)
    {
        auto it = windows.find(handle.id);
        if(it == windows.end())
        {
            return;
        }

        DestroyWindow(it->second.hwnd);
        windows.erase(it);
    }

    void WindowManagerImpl::setTitle(WindowHandle handle, const String& new_title)
    {
        auto it = windows.find(handle.id);
        if(it == windows.end())
        {
            return;
        }

        it->second.title = new_title;
        SetWindowTextA(it->second.hwnd, new_title.c_str());
    }

    void WindowManagerImpl::setSize(WindowHandle handle, U32 new_width, U32 new_height)
    {
        auto it = windows.find(handle.id);
        if(it == windows.end())
        {
            return;
        }

        RECT rect;
        GetClientRect(it->second.hwnd, &rect);
        int dw = static_cast<int>(new_width - (rect.right - rect.left));
        int dh = static_cast<int>(new_height - (rect.bottom - rect.top));

        RECT win_rect;
        GetWindowRect(it->second.hwnd, &win_rect);
        SetWindowPos(it->second.hwnd,
                     nullptr,
                     0,
                     0,
                     win_rect.right - win_rect.left + dw,
                     win_rect.bottom - win_rect.top + dh,
                     SWP_NOMOVE | SWP_NOZORDER);
    }

    void WindowManagerImpl::show(WindowHandle handle)
    {
        auto it = windows.find(handle.id);
        if(it == windows.end())
        {
            return;
        }
        ShowWindow(it->second.hwnd, SW_SHOW);
    }

    void WindowManagerImpl::minimize(WindowHandle handle)
    {
        auto it = windows.find(handle.id);
        if(it == windows.end())
        {
            return;
        }
        ShowWindow(it->second.hwnd, SW_MINIMIZE);
    }

    Bool WindowManagerImpl::shouldClose(WindowHandle handle) const
    {
        auto it = windows.find(handle.id);
        return it != windows.end() && it->second.should_close;
    }

    void WindowManagerImpl::processEvents(WindowHandle handle)
    {
        MSG msg;
        while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    HWND WindowManagerImpl::getHWND(WindowHandle handle) const
    {
        auto it = windows.find(handle.id);
        return (it != windows.end()) ? it->second.hwnd : nullptr;
    }
#elif defined(__linux__)

#endif
} // namespace gg;
