module;

#include <utility>

export module types.pair;

namespace gg
{
    export template<typename T1, typename T2>
    using Pair = std::pair<T1, T2>;
} // namespace gg;
