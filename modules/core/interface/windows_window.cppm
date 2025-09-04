export module core.windows_window;

import core.base_window;
import core.window_descriptor;

export namespace gg
{

class WindowsWindow final : public BaseWindow
{
public:
        WindowsWindow(const WindowDescriptor& descriptor);
        ~WindowsWindow() override = default;

        WindowsWindow(const WindowsWindow&) = delete;
        WindowsWindow& operator=(WindowsWindow&) = delete;

        WindowsWindow(WindowsWindow&&) = delete;
        WindowsWindow& operator=(WindowsWindow&&) = delete;
};

} // namespace gg;
