module;

#include <memory>

export module common:smart_pointers;

namespace gg
{
    export template<typename T>
    using UniquePtr = std::unique_ptr<T>;

    export template<typename T>
    using SharedPtr = std::shared_ptr<T>;

    export template<typename T>
    using WeakPtr = std::weak_ptr<T>;
} // namespace gg;
