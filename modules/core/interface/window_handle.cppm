module;

export module core.window_handle;

import types.base_types;

namespace gg
{
    export struct WindowHandle
    {
        U64 id = 0;

        constexpr Bool operator==(const WindowHandle& other) const noexcept
        {
            return this->id == other.id;
        }

        explicit constexpr operator Bool() const noexcept
        {
            return this->id != 0;
        }
    }; // struct WindowHandle;
} // namespace gg;
