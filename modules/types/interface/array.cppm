module;

#include <array>

export module types.array;

import types.base_types;

namespace gg
{
    export template<typename T, Size N>
    using Array = std::array<T, N>;
} // namespace gg;
