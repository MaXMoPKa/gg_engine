module;

#include <limits>

export module ecs.platform;

import types.base_types;

namespace gg
{
    export using ObjectId = std::size_t;
    export using TypeId = std::size_t;

    export const ObjectId INVALID_OBJECT_ID = std::numeric_limits<ObjectId>::max();
    export const TypeId INVALID_TYPE_ID = std::numeric_limits<ObjectId>::max();

    export union TimeStamp
    {
        F32 as_float;
        U32 as_u32;

        TimeStamp()
            : as_u32{0u}
        {}

        explicit TimeStamp(float value)
            : as_float{value}
        {}

        operator U32() const
        {
            return this->as_u32;
        }

        inline const Bool operator==(const TimeStamp& other) const
        {
             return this->as_u32 == other.as_u32;
        }
        inline const Bool operator!=(const TimeStamp& other) const
        {
            return this->as_u32 != other.as_u32;
        }

        inline const Bool operator<(const TimeStamp& other) const
        {
            return this->as_float < other.as_float;
        }
        inline const Bool operator>(const TimeStamp& other) const
        {
            return this->as_float > other.as_float;
        }
    };
} // namespace gg;
