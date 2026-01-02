module;

#include <unordered_map>

export module types.unordered_map;

namespace gg
{
    export template<typename K, typename V>
    using UnorderedMap = std::unordered_map<K, V>;
} // namespace gg;
