export module core.window_descriptor;

import core.types;
import core.vector;

export namespace gg
{
    struct WindowDescriptor
    {
        public:
            WindowDescriptor() = default;
            WindowDescriptor(i16vec2 size, const String& title = "Title")
                : size{size}
                , title{title}
            {}

            [[nodiscard]] bool is_valid() const
            {
                return size.x > 0 && size.y > 0 && !title.empty();
            }

        public:
            i16vec2 size{800, 600};
            String title{"Title"};
    };
}
