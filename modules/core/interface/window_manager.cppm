module;

export module core.window_manager;

import core.window_descriptor;
import core.window_handle;

import types.base_types;
import types.string;

namespace gg
{
    export class WindowManager
    {
        public:
            WindowManager() = delete;

            static WindowHandle create(const WindowDescriptor& descriptor);
            static void destroy(WindowHandle handle);

            static void setTitle(WindowHandle handle, const String& new_title);
            static void setSize(WindowHandle handle, U32 new_width, U32 new_height);
            static void show(WindowHandle handle);
            static void minimize(WindowHandle handle);

            static Bool shouldClose(WindowHandle handle);
            static void processEvents(WindowHandle handle);
    }; // class WindowManager;
} // namespace gg;
