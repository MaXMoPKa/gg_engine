module;

#include <list>

export module common:list;

namespace gg
{
    export template<typename T>
    using List = std::list<T>;
} // namespace gg;
