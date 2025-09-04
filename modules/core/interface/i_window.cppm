export module core.interface.i_window;

import core.types;

export namespace gg
{
    class IWindow
    {
        public:
            IWindow() = default;
            virtual ~IWindow() = default;

            IWindow(const IWindow&) = delete;
            IWindow& operator=(const IWindow&) = delete;

            IWindow(IWindow&&) = delete;
            IWindow& operator=(IWindow&&) = default;

            virtual void setTitle( const String& new_title) = 0;
    };
} // namespace gg
