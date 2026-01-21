module;

#include <cassert>
#include <functional>

#if defined(_WIN32)
    #include <windows.h>
#elif defined(__linux__)
    #include <X11/Xlib.h>
#endif

module core.window_manager:platform;

import core.window_descriptor;
import core.window_handle;

import input.key;

import types.swizzle_vector;
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

    static std::function<void(Key, Bool)> g_key_callback;
    
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
            void setSize(WindowHandle handle, U32Vec2 new_size);
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

            void requestClose(WindowHandle handle)
            {
                if(contains(handle))
                {
                    windows[handle.id].should_close = true;
                }
            }

            void setKeyCallback(std::function<void(Key, Bool)> callback)
            {
                g_key_callback = std::move(callback);
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

    static Key translateVKToKey(WPARAM vk);
    
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
    {
        using namespace gg;

        WindowManagerImpl& wm = WindowManagerImpl::get();
        PlatformWindowData* target_window = nullptr;
        for (auto& [id, data] : wm.windows) {
            if (data.hwnd == hwnd) {
                target_window = &data;
                break;
            }
        }

        switch (msg)
        {
            case WM_CLOSE:
            case WM_DESTROY:
            {
                if (target_window)
                {
                    target_window->should_close = true;
                }
                return 0;
            }
            case WM_KEYDOWN:
            {
                if (g_key_callback)
                {
                    Key key = translateVKToKey(wp);
                    if (key != Key::Unknown)
                    {
                        g_key_callback(key, true);
                    }
                }
                break;
            }
            default:
                break;
        }

        return DefWindowProc(hwnd, msg, wp, lp);
    }

    static Key translateVKToKey(WPARAM vk)
    {
        switch (vk) {
            case 'A': return Key::A;
            case 'B': return Key::B;
            case 'C': return Key::C;
            case 'D': return Key::D;
            case 'E': return Key::E;
            case 'F': return Key::F;
            case 'G': return Key::G;
            case 'H': return Key::H;
            case 'I': return Key::I;
            case 'J': return Key::J;
            case 'K': return Key::K;
            case 'L': return Key::L;
            case 'M': return Key::M;
            case 'N': return Key::N;
            case 'O': return Key::O;
            case 'P': return Key::P;
            case 'Q': return Key::Q;
            case 'R': return Key::R;
            case 'S': return Key::S;
            case 'T': return Key::T;
            case 'U': return Key::U;
            case 'V': return Key::V;
            case 'W': return Key::W;
            case 'X': return Key::X;
            case 'Y': return Key::Y;
            case 'Z': return Key::Z;

            case '0': return Key::D0;
            case '1': return Key::D1;
            case '2': return Key::D2;
            case '3': return Key::D3;
            case '4': return Key::D4;
            case '5': return Key::D5;
            case '6': return Key::D6;
            case '7': return Key::D7;
            case '8': return Key::D8;
            case '9': return Key::D9;

            case VK_SPACE: return Key::Space;
            case VK_ESCAPE: return Key::Escape;
            case VK_RETURN: return Key::Return;
            case VK_TAB: return Key::Tab;
            case VK_BACK: return Key::Backspace;
            case VK_INSERT: return Key::Insert;
            case VK_DELETE: return Key::Delete;
            case VK_HOME: return Key::Home;
            case VK_END: return Key::End;
            case VK_PRIOR: return Key::PageUp; 
            case VK_NEXT: return Key::PageDown;

            case VK_F1: return Key::F1;
            case VK_F2: return Key::F2;
            case VK_F3: return Key::F3;
            case VK_F4: return Key::F4;
            case VK_F5: return Key::F5;
            case VK_F6: return Key::F6;
            case VK_F7: return Key::F7;
            case VK_F8: return Key::F8;
            case VK_F9: return Key::F9;
            case VK_F10: return Key::F10;
            case VK_F11: return Key::F11;
            case VK_F12: return Key::F12;

            case VK_UP: return Key::Up;
            case VK_DOWN: return Key::Down;
            case VK_LEFT: return Key::Left;
            case VK_RIGHT: return Key::Right;

            default:
                return Key::Unknown;
        }
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
                                   static_cast<int>(descriptor.size.x),
                                   static_cast<int>(descriptor.size.y),
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

    void WindowManagerImpl::setSize(WindowHandle handle, U32Vec2 new_size)
    {
        auto it = windows.find(handle.id);
        if(it == windows.end())
        {
            return;
        }

        RECT rect;
        GetClientRect(it->second.hwnd, &rect);
        int dw = static_cast<int>(new_size.x - (rect.right - rect.left));
        int dh = static_cast<int>(new_size.y - (rect.bottom - rect.top));

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
