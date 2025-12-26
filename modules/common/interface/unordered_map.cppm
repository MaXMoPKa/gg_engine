module;

#include <unordered_map>

export module common:unordered_map;

namespace gg
{
    export template<typename K, typename V>
    using UnorderedMap = std::unordered_map<K, V>;
} // namespace gg;
