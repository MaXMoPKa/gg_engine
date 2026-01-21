module;

export module input.keymap_loader;

import input.key;
import input.key_sequence;

import types.base_types;
import types.vector;
import types.string;
import types.unordered_map;

namespace gg
{
    export class KeyMapLoader
    {
        public:
            void registerAction(const String& name, KeyAction action);
            [[nodiscard]] Bool loadFromJson(const String& json_str, KeyMap& out_map);
        private:
            [[nodiscard]] Key parseKeyString(const String& s);
            UnorderedMap<String, KeyAction> actions;
    }; // class KeyMapLoader;
} // namespace gg;
