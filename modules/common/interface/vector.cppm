module;

#include <vector>

export module common:vector;

namespace gg
{
    export template<typename T>
    using Vector = std::vector<T>;
} // namespace gg;
