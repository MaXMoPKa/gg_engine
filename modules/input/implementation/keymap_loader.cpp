module;

#include <nlohmann/json.hpp>

module input.keymap_loader;

namespace gg
{
    void KeyMapLoader::registerAction(const String& name, KeyAction action)
    {
        this->actions[name] = std::move(action);
    }

    Bool KeyMapLoader::loadFromJson(const String& json_str, KeyMap& out_map)
    {
        try
        {
            auto j = nlohmann::json::parse(json_str);

            if(!j.contains("bindings") || !j["bindings"].is_array())
            {
                return false;
            }

            for(const auto& binding_json : j["bindings"])
            {
                if(!binding_json.contains("keys") || !binding_json.contains("action"))
                {
                    continue;
                }

                Vector<String> key_strings = binding_json["keys"].get<Vector<String>>();
                String action_name = binding_json["action"].get<String>();

                KeySequence sequence;
                for(const String& ks : key_strings)
                {
                    Key key = parseKeyString(ks);
                    if(key == Key::Unknown)
                    {
                        sequence.clear();
                        break;
                    }
                    sequence.push_back(key);
                }

                if(sequence.empty())
                {
                    continue;
                }

                String description = binding_json.value("description", String(""));
                // if action empty -- it is group bind
                if(action_name.empty())
                {
                    out_map.bind(sequence, nullptr, description);
                }
                else
                {
                    auto it = actions.find(action_name);
                    if(it == actions.end())
                    {
                        continue;
                    }

                    out_map.bind(sequence, it->second, description);
                }
            }
            
            return true;
        }
        catch(...)
        {
            return false;
        }
    }

    Key KeyMapLoader::parseKeyString(const String& s)
    {
    
        if (s == "SPC" || s == "space") return Key::Space;
        if (s == "C" || s == "ctrl") return Key::Ctrl;
        if (s == "M" || s == "alt") return Key::Alt;
        if (s == "S" || s == "shift") return Key::Shift;

        if (s == "a") return Key::A;
        if (s == "b") return Key::B;
        if (s == "c") return Key::C;
        if (s == "d") return Key::D;
        if (s == "e") return Key::E;
        if (s == "f") return Key::F;
        if (s == "g") return Key::G;
        if (s == "h") return Key::H;
        if (s == "i") return Key::I;
        if (s == "j") return Key::J;
        if (s == "k") return Key::K;
        if (s == "l") return Key::L;
        if (s == "m") return Key::M;
        if (s == "n") return Key::N;
        if (s == "o") return Key::O;
        if (s == "p") return Key::P;
        if (s == "q") return Key::Q;
        if (s == "r") return Key::R;
        if (s == "s") return Key::S;
        if (s == "t") return Key::T;
        if (s == "u") return Key::U;
        if (s == "v") return Key::V;
        if (s == "w") return Key::W;
        if (s == "x") return Key::X;
        if (s == "y") return Key::Y;
        if (s == "z") return Key::Z;

        if (s == "0") return Key::D0;
        if (s == "1") return Key::D1;
        if (s == "2") return Key::D2;
        if (s == "3") return Key::D3;
        if (s == "4") return Key::D4;
        if (s == "5") return Key::D5;
        if (s == "6") return Key::D6;
        if (s == "7") return Key::D7;
        if (s == "8") return Key::D8;
        if (s == "9") return Key::D9;

        if (s == "ESC" || s == "escape") return Key::Escape;
        if (s == "RET" || s == "return" || s == "enter") return Key::Return;
        if (s == "TAB" || s == "tab") return Key::Tab;
        if (s == "BS" || s == "backspace") return Key::Backspace;
        if (s == "INS" || s == "insert") return Key::Insert;
        if (s == "DEL" || s == "delete") return Key::Delete;
        if (s == "HOME" || s == "home") return Key::Home;
        if (s == "END" || s == "end") return Key::End;
        if (s == "PGUP" || s == "pageup") return Key::PageUp;
        if (s == "PGDN" || s == "pagedown") return Key::PageDown;

        if (s == "f1") return Key::F1;
        if (s == "f2") return Key::F2;
        if (s == "f3") return Key::F3;
        if (s == "f4") return Key::F4;
        if (s == "f5") return Key::F5;
        if (s == "f6") return Key::F6;
        if (s == "f7") return Key::F7;
        if (s == "f8") return Key::F8;
        if (s == "f9") return Key::F9;
        if (s == "f10") return Key::F10;
        if (s == "f11") return Key::F11;
        if (s == "f12") return Key::F12;

        if (s == "up") return Key::Up;
        if (s == "down") return Key::Down;
        if (s == "left") return Key::Left;
        if (s == "right") return Key::Right;

        if (s == "kp0") return Key::Numpad0;
        if (s == "kp1") return Key::Numpad1;
        if (s == "kp2") return Key::Numpad2;
        if (s == "kp3") return Key::Numpad3;
        if (s == "kp4") return Key::Numpad4;
        if (s == "kp5") return Key::Numpad5;
        if (s == "kp6") return Key::Numpad6;
        if (s == "kp7") return Key::Numpad7;
        if (s == "kp8") return Key::Numpad8;
        if (s == "kp9") return Key::Numpad9;
        if (s == "kpRET") return Key::NumpadEnter;
        if (s == "kp+") return Key::NumpadAdd;
        if (s == "kp-") return Key::NumpadSubtract;
        if (s == "kp*") return Key::NumpadMultiply;
        if (s == "kp/") return Key::NumpadDivide;

        return Key::Unknown;
    }

} // namespace gg;
