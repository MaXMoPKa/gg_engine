module;

#include <format>

module input.key_dispatcher;

import types.vector;

namespace gg
{
    void KeyDispatcher::setKeyMap(const KeyMap& map)
    {
        this->key_map = &map;
        this->reset();
    }
    
    void KeyDispatcher::onKeyPress(Key key)
    {
        if(key == Key::Escape)
        {
            this->cancel();
            return;
        }

        this->current_state.current_sequence.push_back(key);
        this->tryDispatch();
    }

    void KeyDispatcher::onKeyRelease(Key key)
    {}

    void KeyDispatcher::cancel()
    {
        this->reset();
        this->current_state.status_message = "Cancelled";
    }

    void KeyDispatcher::reset()
    {
        this->current_state.current_sequence.clear();
        this->current_state.status_message.reset();
        this->current_state.suggestions.clear();
        this->current_state.is_awaiting_input = false;
    }

    void KeyDispatcher::tryDispatch()
    {
        if(!this->key_map)
        {
            return;
        }

        if(const KeyAction* action = this->key_map->findExact(this->current_state.current_sequence))
        {
            (*action)();
            this->reset();
            return;
        }

        Vector<Suggestion> suggestions = this->key_map->getSuggestions(this->current_state.current_sequence);
        if(!suggestions.empty())
        {
            this->current_state.suggestions = std::move(suggestions);
            this->current_state.is_awaiting_input = true;

            String msg = std::format("Key: {}\n", this->formatSequence(this->current_state.current_sequence));
            for(const auto& suggestion : this->current_state.suggestions)
            {
                msg += std::format("  {} -> {}\n", this->formatKey(suggestion.next_key), suggestion.description);
            }
            this->current_state.status_message = std::move(msg);
        }
        else
        {
            this->current_state.status_message = std::format("Invalid key sequence: {}", this->formatSequence(this->current_state.current_sequence));
            this->current_state.suggestions.clear();
            this->current_state.is_awaiting_input = false;
            this->current_state.current_sequence.clear();
        }
    }

    String KeyDispatcher::formatSequence(const KeySequence& sequence) const
    {
        String out;
        for(Size i = 0; i < sequence.size(); ++i)
        {
            if(i > 0)
            {
                out += " ";
            }
            out += this->formatKey(sequence[i]);
        }

        return out;
    }

    String KeyDispatcher::formatKey(Key key) const
    {
        switch(key)
        {
            case Key::Ctrl:  return "C-";
            case Key::Alt:   return "M-";
            case Key::Shift: return "S-";
            case Key::Space: return "SPC";

            case Key::A: return "a";
            case Key::B: return "b";
            case Key::C: return "c";
            case Key::D: return "d";
            case Key::E: return "e";
            case Key::F: return "f";
            case Key::G: return "g";
            case Key::H: return "h";
            case Key::I: return "i";
            case Key::J: return "j";
            case Key::K: return "k";
            case Key::L: return "l";
            case Key::M: return "m";
            case Key::N: return "n";
            case Key::O: return "o";
            case Key::P: return "p";
            case Key::Q: return "q";
            case Key::R: return "r";
            case Key::S: return "s";
            case Key::T: return "t";
            case Key::U: return "u";
            case Key::V: return "v";
            case Key::W: return "w";
            case Key::X: return "x";
            case Key::Y: return "y";
            case Key::Z: return "z";
                
            case Key::D0: return "0";
            case Key::D1: return "1";
            case Key::D2: return "2";
            case Key::D3: return "3";
            case Key::D4: return "4";
            case Key::D5: return "5";
            case Key::D6: return "6";
            case Key::D7: return "7";
            case Key::D8: return "8";
            case Key::D9: return "9";
                
            case Key::Escape:    return "ESC";
            case Key::Return:    return "RET";
            case Key::Tab:       return "TAB";
            case Key::Backspace: return "BS";
            case Key::Insert:    return "INS";
            case Key::Delete:    return "DEL";
            case Key::Home:      return "HOME";
            case Key::End:       return "END";
            case Key::PageUp:    return "PGUP";
            case Key::PageDown:  return "PGDN";
                
            case Key::F1:  return "f1";
            case Key::F2:  return "f2";
            case Key::F3:  return "f3";
            case Key::F4:  return "f4";
            case Key::F5:  return "f5";
            case Key::F6:  return "f6";
            case Key::F7:  return "f7";
            case Key::F8:  return "f8";
            case Key::F9:  return "f9";
            case Key::F10: return "f10";
            case Key::F11: return "f11";
            case Key::F12: return "f12";
                
            case Key::Up:    return "↑";
            case Key::Down:  return "↓";
            case Key::Left:  return "←";
            case Key::Right: return "→";
                
            case Key::Comma:        return ","; 
            case Key::Period:       return "."; 
            case Key::Slash:        return "/"; 
            case Key::Semicolon:    return ";"; 
            case Key::Quote:        return "'"; 
            case Key::Backquote:    return "`"; 
            case Key::Backslash:    return "\\"; 
            case Key::LeftBracket:  return "["; 
            case Key::RightBracket: return "]"; 
            case Key::Minus:        return "-"; 
            case Key::Equals:       return "="; 

            case Key::Numpad0:        return "kp0"; 
            case Key::Numpad1:        return "kp1"; 
            case Key::Numpad2:        return "kp2"; 
            case Key::Numpad3:        return "kp3"; 
            case Key::Numpad4:        return "kp4"; 
            case Key::Numpad5:        return "kp5"; 
            case Key::Numpad6:        return "kp6"; 
            case Key::Numpad7:        return "kp7"; 
            case Key::Numpad8:        return "kp8"; 
            case Key::Numpad9:        return "kp9"; 
            case Key::NumpadEnter:    return "kpRET"; 
            case Key::NumpadAdd:      return "kp+"; 
            case Key::NumpadSubtract: return "kp-"; 
            case Key::NumpadMultiply: return "kp*"; 
            case Key::NumpadDivide:   return "kp/";

            case Key::Unknown:
            default: return "?";
        }      
    }

    KeyDispatcher::State KeyDispatcher::getState() const
    {
        return this->current_state;
    }
} // namespace gg;
