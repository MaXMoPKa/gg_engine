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
