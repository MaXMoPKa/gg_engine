module;

#include <set>

export module common:set;

namespace gg
{
    export template<typename T>
    using Set = std::set<T>;
} // namespace gg;
