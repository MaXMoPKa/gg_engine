module;

#include <exception>
#include <thread>

#define WIND32_LEAN_AND_MEAN
#include <Windows.h>
#include <comdef.h>

export module gg.render.helpers;

export namespace gg
{

inline void throwIfFailed(HRESULT hr)
{
    if(FAILED(hr))
    {
        _com_error err(hr);
        OutputDebugString(err.ErrorMessage());

        throw std::exception(err.ErrorMessage());
    }
}

const DWORD MS_VC_EXCEPTION = 0x406D1388;

#pragma pack(push, 8)
typedef struct tagTHREADNAME_INFO
{
    DWORD dwType;     // Must be 0x1000.
    LPCSTR szName;    // Pointer to name (in user addr space).
    DWORD dwThreadId; // Thread ID (-1 = called thread).
    DWORD dwFlags;    // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

inline void setThreadName(std::thread& thread, const char* thread_name)
{
    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = thread_name;
    info.dwThreadId = ::GetThreadId(reinterpret_cast<HANDLE>(thread.native_handle()));
    info.dwFlags = 0;

    __try
    {
        ::RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
    }
}

}

export namespace math
{

constexpr float PI = 3.1415926535897932384626433832795f;
constexpr float PI_2 = 2.0f * PI;
constexpr float radToDeg(const float radians)
{
    return radians * (180.0f / PI);
}

constexpr float degToRad(const float degrees)
{
    return degrees * (PI / 180.0f);
}

template<typename T>
inline T deadzone(T val, T deadzone)
{
    if (std::abs(val) < deadzone)
    {
        return T(0);
    }
    return val;
}

template<typename T, typename U>
inline T normalizeRange(U x, U min, U max)
{
    return T(x - min) / T(max - min);
}

template<typename T, typename U>
inline T shiftBias(U x, U shift, U bias)
{
    return T(x * bias) + T(shift);
}

template <typename T>
inline T alignUpWithMask(T value, size_t mask)
{
    return (T)(((size_t)value + mask) & ~mask);
}

template <typename T>
inline T alignDownWithMask(T value, size_t mask)
{
    return (T)((size_t)value & ~mask);
}

template <typename T>
inline T alignUp(T value, size_t alignment)
{
    return alignUpWithMask(value, alignment - 1);
}

template <typename T>
inline T alignDown(T value, size_t alignment)
{
    return alignDownWithMask(value, alignment - 1);
}

template <typename T>
inline bool isAligned(T value, size_t alignment)
{
    return 0 == ((size_t)value & (alignment - 1));
}

template <typename T>
inline T divideByMultiple(T value, size_t alignment)
{
    return (T)((value + alignment - 1) / alignment);
}

inline uint32_t nextHighestPow2(uint32_t v)
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;

    return v;
}

inline uint64_t nextHighestPow2(uint64_t v)
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v |= v >> 32;
    v++;

    return v;
}

} // namespace math;
