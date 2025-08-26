#include "src/include/dx11_graphics.hpp"

#include <d3d11.h>
#include <d3dcommon.h>
#include <dxgi.h>
#include <dxgiformat.h>
#include <dxgitype.h>
#include <ostream>
#include <winerror.h>
#include <winscard.h>

#include "third_party/dxerr/dxerr.h"

#include <sstream>

#define GFX_EXCEPT_NOINFO(hr) DX11Graphics::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw DX11Graphics::HrException( __LINE__,__FILE__,hr )

#ifndef NDEBUG
#define GFX_EXCEPT(hr) DX11Graphics::HrException( __LINE__,__FILE__,(hr), info_manager.getMessages() )
#define GFX_THROW_INFO(hrcall) info_manager.set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) DX11Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr),info_manager.getMessages() )
#else
#define GFX_EXCEPT(hr) DX11Graphics::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) DX11Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr) )
#endif

gg::gapi::DX11Graphics::DX11Graphics(HWND h_window)
{
    DXGI_SWAP_CHAIN_DESC swap_chain_desc = {};
    swap_chain_desc.BufferDesc.Width = 0;
    swap_chain_desc.BufferDesc.Height = 0;
    swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swap_chain_desc.BufferDesc.RefreshRate.Numerator = 0;
    swap_chain_desc.BufferDesc.RefreshRate.Denominator = 0;
    swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swap_chain_desc.SampleDesc.Count = 1;
    swap_chain_desc.SampleDesc.Quality = 0;
    swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_desc.BufferCount = 1;
    swap_chain_desc.OutputWindow = h_window;
    swap_chain_desc.Windowed = TRUE;
    swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swap_chain_desc.Flags = 0;

    UINT swap_create_flags = 0u;
#ifndef NDEBUG
    swap_create_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    HRESULT hr = 0;

    GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
                                  swap_create_flags, nullptr, 0, D3D11_SDK_VERSION, &swap_chain_desc, &p_swap_chain,
                                  &p_device, nullptr, &p_context));

    Microsoft::WRL::ComPtr<ID3D11Resource> back_buffer = nullptr;
    GFX_THROW_INFO(p_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), &back_buffer));
    GFX_THROW_INFO(p_device->CreateRenderTargetView(back_buffer.Get(), nullptr, &p_target));
}

void gg::gapi::DX11Graphics::endFrame()
{
    HRESULT hr = 0;
    if(FAILED(hr = p_swap_chain->Present(1u, 0u)))
    {
#ifndef NDEBUG
        info_manager.set();
#endif
        if(hr == DXGI_ERROR_DEVICE_REMOVED)
        {
            throw GFX_DEVICE_REMOVED_EXCEPT(p_device->GetDeviceRemovedReason());
        }
        else
        {
            GFX_EXCEPT(hr);
        }
    }
}

void gg::gapi::DX11Graphics::clearBuffer(float red, float green, float blue) noexcept
{
    const float color[] = {red, green, blue, 1.0f};
    p_context->ClearRenderTargetView(p_target.Get(), color);
}

gg::gapi::DX11Graphics::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> info_messages) noexcept
    : GraphicsException(line, file)
    , hr(hr)
{
    for (const auto& m : info_messages)
    {
        info += m;
        info.push_back('\n');
    }
    if (!info.empty())
    {
        info.pop_back();
    }
}

const char* gg::gapi::DX11Graphics::HrException::what() const noexcept
{
    std::ostringstream oss;
    oss << getType() << std::endl
        << "[Error Code] 0x" << std::hex << std::uppercase << getErrorCode()
        << std::dec << " (" << (unsigned long)getErrorCode() << ")" << std::endl
        << "[Error String] " << getErrorString() << std::endl
        << "[Description] " << getErrorDescription() << std::endl;
    if (!info.empty())
    {
        oss << "\n[Error Info]\n" << getErrorInfo() << std::endl << std::endl;
    }
    oss << getOriginString();
    what_buffer = oss.str();
    return what_buffer.c_str();
}

const char* gg::gapi::DX11Graphics::HrException::getType() const noexcept
{
    return "Graphics Exception";
}

HRESULT gg::gapi::DX11Graphics::HrException::getErrorCode() const noexcept
{
    return hr;
}

std::string gg::gapi::DX11Graphics::HrException::getErrorString() const noexcept
{
    return DXGetErrorString(hr);
}

std::string gg::gapi::DX11Graphics::HrException::getErrorDescription() const noexcept
{
    char buf[512];
    DXGetErrorDescription(hr, buf, sizeof(buf));
    return buf;
}

std::string gg::gapi::DX11Graphics::HrException::getErrorInfo() const noexcept
{
    return info;
}

const char* gg::gapi::DX11Graphics::DeviceRemovedException::getType() const noexcept
{
    return "Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}
