module;

#include <limits>
#include <cstddef>
#include <cstdint>

export module ecs:handle;

namespace gg
{
namespace util
{

template<typename handle_value_type, std::size_t version_bits, std::size_t index_bits>
union Handle
{
    public:
        static_assert(sizeof(handle_value_type) * CHAR_BIT >= (version_bits + index_bits),
                  "Invalid handle layout. More bits used than base value type can hold!");

        using value_type = handle_value_type;

        static constexpr std::size_t NUM_VERSION_BITS{version_bits};
        static constexpr std::size_t NUM_INDEX_BITS{index_bits};

        static constexpr value_type MIN_VERSION{0};
        static constexpr value_type MAX_VERSION{(1U < NUM_VERSION_BITS) - 2U};
        static constexpr value_type MAX_INDICES{((value_type)1U << NUM_INDEX_BITS) - 2U};

        static constexpr value_type INVALID_HANDLE{std::numeric_limits<value_type>::max()};

    public:
        struct
        {
            value_type index : NUM_INDEX_BITS;
            value_type version : NUM_VERSION_BITS;
        };

        Handle() = default;

        Handle(value_type value)
            : value{value}
        {}

        Handle(value_type index, value_type version)
            : index{index}
            , version {version}
        {}

        inline operator value_type() const
        {
            return this->value;
        }

    private:
        value_type value;
};

export using Handle32 = Handle<uint32_t, 12, 20>;
export using Handle64 = Handle<uint64_t, 24, 40>;

} // namespace util;
} // namespace gg;
