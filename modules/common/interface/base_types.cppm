module;

#include <cstdint>
#include <uchar.h>

export module common:base_types;

namespace gg
{
    export using U8 = std::uint8_t;
    export using U16 = std::uint16_t;
    export using U32 = std::uint32_t;
    export using U64 = std::uint64_t;

    export using I8 = std::int8_t;
    export using I16 = std::int16_t;
    export using I32 = std::int32_t;
    export using I64 = std::int64_t;

    export using F32 = float;
    export using F64 = double;
    export using F128 = long double;

    export using Bool = bool;

    export using Char = char;
    export using SChar = signed char;
    export using UChar = unsigned char;
    export using WChar = wchar_t;
    export using Char8 = char8_t;
    export using Char16 = char16_t;
    export using Char32 = char32_t;
    export using Size = size_t;
    export using IntPtr = std::intptr_t;
    export using UIntPtr = std::uintptr_t;
} // namespace gg;
