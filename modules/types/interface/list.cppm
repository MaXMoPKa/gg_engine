module;

#include <list>

export module types.list;

namespace gg
{
    export template<typename T>
    using List = std::list<T>;
} // namespace gg;
