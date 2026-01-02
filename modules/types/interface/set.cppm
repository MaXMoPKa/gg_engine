module;

#include <set>

export module types.set;

namespace gg
{
    export template<typename T>
    using Set = std::set<T>;
} // namespace gg;
