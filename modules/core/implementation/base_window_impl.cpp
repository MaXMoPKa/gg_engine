module core.base_window;

import core.window_descriptor;

namespace gg
{

BaseWindow::BaseWindow(const WindowDescriptor& descriptor)
    : IWindow{}
    , descriptor{descriptor}
{}

void BaseWindow::setTitle(const String& new_title)
{
    if(!new_title.empty())
    {
        descriptor.title = new_title;
    }
}

}
