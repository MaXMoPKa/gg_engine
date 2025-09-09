module;

export module core.base_window;

import core.types;
import core.interface.i_window;
import core.window_descriptor;

export namespace gg
{
    class BaseWindow : public IWindow
    {
        public:
            BaseWindow(const WindowDescriptor& descriptor)
                : IWindow{}
                , descriptor{descriptor}
            {}

            ~BaseWindow() override = default;

            BaseWindow(const BaseWindow&) = delete;
            BaseWindow& operator=(const BaseWindow&) = delete;

            BaseWindow(BaseWindow&&) = delete;
            BaseWindow& operator=(BaseWindow&&) = delete;

            String& getTitle()
            {
                return descriptor.title;
            }
            String getTitle() const
            {
                return descriptor.title;
            }
            void setTitle(const String& new_title) override
            {
                if(!new_title.empty())
                {
                    descriptor.title = new_title;
                }
            }

        protected:
            WindowDescriptor descriptor;
    };
} // namespace gg
