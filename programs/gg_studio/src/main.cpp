#include <iostream>

import core.window_manager;
import core.window_handle;

import input.key;
import input.key_dispatcher;
import input.key_sequence;

import types.base_types;
import types.string;

int main()
{
    using namespace gg;
    
    WindowHandle handle = WindowManager::create({.width = 800, .height = 600, .title = "GG Studio"});
    if(!handle)
    {
        std::cerr << "Failed to create window!" << std::endl;
        return -1;
    }

    KeyMap keymap;
    keymap.bind({Key::Space, Key::Q}, nullptr, "+Quit/Session");
    keymap.bind({Key::Space, Key::F}, nullptr, "+File");
    
    keymap.bind({Key::Space, Key::Q, Key::Q}, [&](){
        std::cout << "Close command triggered!" << std::endl;
        WindowManager::requestClose(handle);
    }, "Quit application");
    keymap.bind({Key::Space, Key::Q, Key::R}, [](){}, "Restart");

    keymap.bind({Key::Space, Key::F, Key::S}, [](){}, "Save");
    

    KeyDispatcher dispatcher;
    dispatcher.setKeyMap(keymap);

    WindowManager::setKeyCallback([&](Key key, Bool pressed)
    {
        if(pressed)
        {
            std::cout << "Pressed: " << static_cast<U16>(key) << std::endl;
            dispatcher.onKeyPress(key);
        }
    });
    
    while(!WindowManager::shouldClose(handle))
    {
        WindowManager::processEvents(handle);

        KeyDispatcher::State state = dispatcher.getState();
        if(state.status_message)
        {
            static String last_msg;
            if(last_msg != state.status_message.value())
            {
                std::cout << state.status_message.value() << std::endl;
                last_msg = state.status_message.value();
            }
        }
    }

    WindowManager::destroy(handle);
    return 0;
}
