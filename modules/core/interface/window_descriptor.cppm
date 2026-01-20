module;

export module core.window_descriptor;

import types.base_types;
import types.string;

namespace gg
{
    export struct WindowDescriptor
    {
        U32 width = 1280u;
        U32 height = 720u;
        String title{"Sandbox"};
    }; // struct WindowDescriptor;

} // namespace gg;
