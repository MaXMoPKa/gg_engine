module;

#include <limits>
#include <cstddef>

export module ecs:platform;

namespace gg
{
    export using ObjectId = std::size_t;
    export using TypeId = std::size_t;

    export const ObjectId INVALID_OBJECT_ID = std::numeric_limits<ObjectId>::max();
    export const TypeId INVALID_TYPE_ID = std::numeric_limits<ObjectId>::max();
} // namespace gg;
