module;

#include <utility>

module input.key_sequence;

namespace gg
{
    void KeyMap::bind(const KeySequence& sequence, KeyAction action, const String& description)
    {
        if(!sequence.empty())
        {
            KeyAction stored_action = action ? std::move(action) : nullptr;
            bindings[sequence] = KeyBinding{stored_action, description};
        }
    }

    const KeyAction* KeyMap::findExact(const KeySequence& sequence) const
    {
        auto it = bindings.find(sequence);
        if(it != bindings.end() && it->second.action)
        {
            return &it->second.action;
        }
        else
        {
            return nullptr;
        }
    }

    Vector<Suggestion> KeyMap::getSuggestions(const KeySequence& prefix) const
    {
        Vector<Suggestion> results;
        Map<Key, Suggestion> seen;

        for(const auto& [seq, binding] : bindings)
        {
            if(seq.size() <= prefix.size())
            {
                continue;
            }

            Bool matches = true;
            for(Size i = 0; i < prefix.size(); ++i)
            {
                if(i >= seq.size() || seq[i] != prefix[i])
                {
                    matches = false;
                    break;
                }
            }
            if(!matches)
            {
                continue;
            }

            Key next_key = seq[prefix.size()];
            if(seen.find(next_key) != seen.end())
            {
                continue;
            }

            Bool is_leaf = (seq.size() == prefix.size() + 1);
            String desc;
            if(!is_leaf)
            {
                KeySequence group_key = prefix;
                group_key.push_back(next_key);
                auto it = bindings.find(group_key);
                if(it != bindings.end() && !it->second.description.empty())
                {
                    desc = it->second.description;
                }
                else
                {
                    desc = "+Unknown";
                }
            }
            else
            {
                desc = binding.description;
                if(desc.empty())
                {
                    desc = "-> Command";
                }
            }

            seen[next_key] = Suggestion{next_key, desc, is_leaf};
        }

        for(const auto& kv : seen)
        {
            results.push_back(kv.second);
        }
        return results;
    }
} // namespace gg;
