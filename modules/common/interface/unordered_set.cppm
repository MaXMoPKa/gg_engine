module;

#include <unordered_set>

export module common:unordered_set;

namespace gg
{
    export template<typename T>
    using UnorderedSet = std::unordered_set<T>;
} // namespace gg;
