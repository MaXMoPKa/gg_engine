module;

export module core.window_descriptor;

import types.swizzle_vector;
import types.string;

namespace gg
{
    export struct WindowDescriptor
    {
        U32Vec2 size {1280u, 720u};
        String title{"Sandbox"};
    }; // struct WindowDescriptor;

} // namespace gg;
