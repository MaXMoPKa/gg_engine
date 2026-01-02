module;

#include <functional>

export module types.function;

namespace gg
{
    export template<typename T>
    using Function = std::function<T>;
} // namespace gg;
