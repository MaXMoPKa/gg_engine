module;

#include <vector>

export module types.vector;

namespace gg
{
    export template<typename T>
    using Vector = std::vector<T>;
} // namespace gg;
