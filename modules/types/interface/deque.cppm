module;

#include <deque>

export module types.deque;

namespace gg
{
    export template<typename T>
    using Deque = std::deque<T>;
} // namespace gg;
