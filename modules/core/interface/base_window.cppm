export module core.base_window;

import core.types;
import core.interface.i_window;
import core.window_descriptor;

export namespace gg
{
    class BaseWindow : public IWindow
    {
        public:
            BaseWindow(const WindowDescriptor& descriptor);
            ~BaseWindow() override = default;

            BaseWindow(const BaseWindow&) = delete;
            BaseWindow& operator=(const BaseWindow&) = delete;

            BaseWindow(BaseWindow&&) = delete;
            BaseWindow& operator=(BaseWindow&&) = delete;

            void setTitle(const String& new_title) override;

        private:
            WindowDescriptor descriptor;
    };
} // namespace gg
