module;

#include <array>

export module common:array;

import :base_types;

namespace gg
{
    export template<typename T, Size N>
    using Array = std::array<T, N>;
} // namespace gg;
