module;

#include <limits>
#include <cstddef>
#include <cstdint>

export module ecs:platform;

namespace gg
{
    export using ObjectId = std::size_t;
    export using TypeId = std::size_t;

    export const ObjectId INVALID_OBJECT_ID = std::numeric_limits<ObjectId>::max();
    export const TypeId INVALID_TYPE_ID = std::numeric_limits<ObjectId>::max();

    export union TimeStamp
    {
        float as_float;
        uint32_t as_uint32;

        TimeStamp()
            : as_uint32{0u}
        {}

        explicit TimeStamp(float value)
            : as_float{value}
        {}

        operator uint32_t() const
        {
            return this->as_uint32;
        }

        inline const bool operator==(const TimeStamp& other) const
        {
             return this->as_uint32 == other.as_uint32;
        }
        inline const bool operator!=(const TimeStamp& other) const
        {
            return this->as_uint32 != other.as_uint32;
        }

        inline const bool operator<(const TimeStamp& other) const
        {
            return this->as_float < other.as_float;
        }
        inline const bool operator>(const TimeStamp& other) const
        {
            return this->as_float > other.as_float;
        }
    };
} // namespace gg;
