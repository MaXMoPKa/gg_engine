module;

#include <string>

export module types.string;

namespace gg
{
    export using String = std::string;
    export using WString = std::wstring;
    export using U8String = std::u8string;
    export using U16String = std::u16string;
    export using U32String = std::u32string;
} // namespace gg;
