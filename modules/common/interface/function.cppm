module;

#include <functional>

export module common:function;

namespace gg
{
    export template<typename T>
    using Function = std::function<T>;
} // namespace gg;
