module;

#include <exception>

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
}
