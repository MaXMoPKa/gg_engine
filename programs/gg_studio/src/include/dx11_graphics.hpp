#pragma once

#include "src/include/dxgi_info_manager.hpp"
#include "src/include/exception.hpp"
#include "src/include/window.h"

#include <d3d11.h>
#include <dxgi.h>


namespace gg
{

namespace gapi
{

class DX11Graphics
{
public:
    class GraphicsException : public Exception
    {
        using Exception::Exception;
    };

    class HrException : public GraphicsException
    {
    public:
        HrException(int line, const char* file, HRESULT hr, std::vector<std::string> info_messages = {}) noexcept;
        const char* what() const noexcept override;
        const char* getType() const noexcept override;
        HRESULT getErrorCode() const noexcept;
        std::string getErrorString() const noexcept;
        std::string getErrorDescription() const noexcept;
        std::string getErrorInfo() const noexcept;
    private:
        HRESULT hr = 0;
        std::string info;
    };

    class DeviceRemovedException : public HrException
    {
        using HrException::HrException;
    public:
        const char* getType() const noexcept override;
    private:
        std::string reason;
    };
public:
    DX11Graphics(HWND h_window);
    DX11Graphics(const DX11Graphics&) = delete;
    DX11Graphics& operator=(const DX11Graphics&) = delete;
    ~DX11Graphics();

    void endFrame();
    void clearBuffer( float red, float green, float blue) noexcept;

private:
#ifndef NDEBUG
    DXGIInfoManager info_manager;
#endif
    ID3D11Device* p_device = nullptr;
    IDXGISwapChain* p_swap_chain = nullptr;
    ID3D11DeviceContext* p_context = nullptr;
    ID3D11RenderTargetView* p_target = nullptr;
};

}

}
