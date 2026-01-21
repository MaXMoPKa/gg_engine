module;

#include <map>

export module types.map;

namespace gg
{
    export template<typename K, typename V, typename Comp = std::less<K>>
    using Map = std::map<K, V, Comp>;
} // namespace gg;
