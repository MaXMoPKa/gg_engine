module;

export module ecs:family_type_id;

import :platform;

namespace gg
{
namespace util
{

export template<class T>
class FamilyTypeId
{
public:
    template<class U>
    static const TypeId get()
    {
        static const TypeId STATIC_TYPE_ID{ s_count++ };
        return STATIC_TYPE_ID;
    }

    static TypeId get()
    {
        return s_count;
    }
private:
    inline static TypeId s_count{0};
};

} // namespace util;
} // namespace gg;
