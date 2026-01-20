module;

module core.window_manager;

import :platform;

namespace gg
{
    WindowHandle WindowManager::create(const WindowDescriptor& descriptor)
    {
        return WindowManagerImpl::get().create(descriptor);
    }

    void WindowManager::destroy(WindowHandle handle)
    {
        WindowManagerImpl::get().destroy(handle);
    }

    void WindowManager::setTitle(WindowHandle handle, const String& title)
    {
        WindowManagerImpl::get().setTitle(handle, title);
    }

    void WindowManager::setSize(WindowHandle handle, U32 new_width, U32 new_height)
    {
        WindowManagerImpl::get().setSize(handle, new_width, new_height);
    }

    void WindowManager::show(WindowHandle handle)
    {
        WindowManagerImpl::get().show(handle);
    }

    void WindowManager::minimize(WindowHandle handle)
    {
        WindowManagerImpl::get().minimize(handle);
    }

    Bool WindowManager::shouldClose(WindowHandle handle)
    {
        return WindowManagerImpl::get().shouldClose(handle);
    }

    void WindowManager::processEvents(WindowHandle handle)
    {
        WindowManagerImpl::get().processEvents(handle);
    }
} // namespace gg;
