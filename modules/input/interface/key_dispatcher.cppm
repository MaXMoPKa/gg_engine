module;

export module input.key_dispatcher;

import input.key;
import input.key_sequence;

import types.base_types;
import types.optional;
import types.string;
import types.vector;

namespace gg
{
    export class KeyDispatcher
    {
        public:
            struct State
            {
                KeySequence current_sequence;
                Optional<String> status_message;
                Vector<Suggestion> suggestions;
                Bool is_awaiting_input = false;
            }; // struct State;

            void setKeyMap(const KeyMap& map);
            void onKeyPress(Key key);
            void onKeyRelease(Key key);

            void cancel();
            State getState() const;

        private:
            void reset();
            void tryDispatch();
            String formatSequence(const KeySequence& sequence) const;
            String formatKey(Key key) const;

            KeySequence current_sequence;
            const KeyMap* key_map = nullptr;
            Optional<String> status_msg;
            Vector<Suggestion> suggestions;
            Bool awaiting_input = false;
    }; // class KeyDispatcher;
} // namespace gg;
