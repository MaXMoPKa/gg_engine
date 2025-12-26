module;

#include <map>

export module common:map;

namespace gg
{
    export template<typename K, typename V>
    using Map = std::map<K, V>;
} // namespace gg;
