#include <iostream>
#include <fstream>
#include <iterator>
#include <stdexcept>

import core.window_manager;
import core.window_handle;

import input.key;
import input.key_dispatcher;
import input.key_sequence;
import input.keymap_loader;

import types.base_types;
import types.string;

gg::String readFile(const gg::String& file_path)
{
    std::ifstream file(file_path);
    if(!file.is_open())
    {
        throw std::runtime_error("Cannot open file: " + file_path);
    }

    return gg::String(std::istreambuf_iterator<gg::Char>(file),
                      std::istreambuf_iterator<gg::Char>());
}

int main()
{
    using namespace gg;
    
    WindowHandle handle = WindowManager::create({.size = {800u, 600u}, .title = "GG Studio"});
    if(!handle)
    {
        std::cerr << "Failed to create window!" << std::endl;
        return -1;
    }

    String json_str = readFile("assets/bindings/shortcuts.json");
    
    KeyMapLoader loader;
    loader.registerAction("quit", [&](){
        WindowManager::requestClose(handle); 
    });
    
    KeyMap keymap;
    if(!loader.loadFromJson(json_str, keymap))
    {
        std::cerr << "Failed to load shortcuts!" << std::endl;
        return -1;
    }
    
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
