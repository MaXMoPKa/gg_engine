module;

#include <limits>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <vector>
#include <algorithm>
#include <cassert>

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

export template<class T, class handle_type, std::size_t grow = 1024>
class HandleTable
{
private:
    using Handle = handle_type;

public:
    HandleTable()
    {
        this->growTable();
    }
    ~HandleTable() = default;

    [[nodiscard]] Handle acquireHandle(T* raw_object)
    {
        typename Handle::value_type i = 0;
        for(; i < this->table.size(); ++i)
        {
            if(this->table[i].second == nullptr)
            {
                this->table[i].second = raw_object;
                this->table[i].first = ((this->table[i].first + 1) > Handle::MAX_VERSION) ? Handle::MIN_VERSION : this->table[i].first + 1;
                return Handle(i, this->table[i].first);
            }
        }
        this->growTable();

        this->table[i].first = 1;
        this->table[i].second = raw_object;

        return Handle(i, this->table[i].first);
    }

    void releaseHandle(Handle handle)
    {
        assert(handle.index < this->table.size() && handle.version == this->table[handle.index].first && "Invalid handle!");
        this->table[handle.index].second = nullptr;
    }

    [[nodiscard]] bool isExpired(Handle handle) const
    {
        return this->table[handle.index].first != handle.version;
    }

    [[nodiscard]] Handle operator[](typename Handle::value_type index) const
    {
        assert(index < this->table.size() && "Invalid handle!");
        return Handle(index, this->table[index].first);
    }

    [[nodiscard]] T* operator[](Handle handle)
    {
        assert(handle.index < this->table.size() && handle.version == this->table[handle.index].first && "Invalid handle!");
        return this->table[handle.index].first == handle.version ? this->table[handle.index].second : nullptr;
    }

private:
    void growTable()
    {
        std::size_t old_size = this->table.size();
        assert(old_size < Handle::MAX_INDICES && "Max table capacity reached!");

        std::size_t new_size = std::min(old_size + grow, (size_t)Handle::MAX_INDICES);

        this->table.resize(new_size);

        for(typename Handle::value_type i = old_size; i < new_size; ++i)
        {
            this->table[i] = TableEntry(Handle::MIN_VERSION, nullptr);
        }
    }

private:
    using TableEntry = std::pair<typename Handle::value_type, T*>;
    std::vector<TableEntry> table;
};

} // namespace util;
} // namespace gg;
