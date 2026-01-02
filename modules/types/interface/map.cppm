module;

#include <map>

export module types.map;

namespace gg
{
    export template<typename K, typename V>
    using Map = std::map<K, V>;
} // namespace gg;
