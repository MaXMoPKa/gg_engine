module;

#include <optional>

export module types.optional;

namespace gg
{
    export template<typename T>
    using Optional = std::optional<T>;
} // namespace gg;
