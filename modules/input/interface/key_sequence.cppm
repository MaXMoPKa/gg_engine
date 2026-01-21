module;

#include <functional>
#include <map>

export module input.key_sequence;

import input.key;

import types.base_types;
import types.string;
import types.vector;
import types.map;

namespace gg
{
    export using KeySequence = Vector<Key>;
    struct KeySequenceLess
    {
        Bool operator()(const KeySequence& a, const KeySequence& b) const
        {
            return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
        }
    }; // struct KeySequenceLess;
    export using KeyAction = std::function<void()>;

    export struct KeyBinding {
        KeyAction action;
        String description;
    }; // struct KeyBinding;

    export struct Suggestion {
        Key next_key;
        String description;
        Bool is_leaf;
    }; // struct Suggestion;
    
    export class KeyMap
    {
        public:
            void bind(const KeySequence& sequence, KeyAction action, const String& description = "");
            [[nodiscard]] const KeyAction* findExact(const KeySequence& sequence) const;
            [[nodiscard]] Vector<Suggestion> getSuggestions(const KeySequence& prefix) const;
        private:
            using KeyBindingMap = Map<KeySequence, KeyBinding, KeySequenceLess>;
            KeyBindingMap bindings;
    }; // class KeyMap;
}; // namespace gg;
