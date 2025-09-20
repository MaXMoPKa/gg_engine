module;

#include <memory>

#include <wrl.h>

#include <d3d12.h>
#include <dxgi1_6.h>

module render;

import :adapter;

import gg.render.helpers;

namespace gg
{

Adapter::AdapterList Adapter::getAdapters(DXGI_GPU_PREFERENCE gpu_preference)
{
    using namespace Microsoft::WRL;

    AdapterList adapters;

    ComPtr<IDXGIFactory6> dxgi_factory_6;
    ComPtr<IDXGIAdapter> dxgi_adapter;
    ComPtr<IDXGIAdapter4> dxgi_adapter_4;

    UINT create_factory_flags = 0;

    #if defined (_DEBUG)
        create_factory_flags = DXGI_CREATE_FACTORY_DEBUG;
    #endif

    for(UINT i = 0; dxgi_factory_6->EnumAdapterByGpuPreference(i, gpu_preference, IID_PPV_ARGS(&dxgi_adapter)) != DXGI_ERROR_NOT_FOUND; ++i)
    {
        if(SUCCEEDED(D3D12CreateDevice(dxgi_adapter.Get(), D3D_FEATURE_LEVEL_12_2, __uuidof(ID3D12Device), nullptr)))
        {
            throwIfFailed(dxgi_adapter.As(&dxgi_adapter_4));
            std::shared_ptr<Adapter> adapter = std::make_shared<MakeAdapter>(dxgi_adapter_4);
            adapters.push_back(adapter);
        }
    }

    return adapters;
}

std::shared_ptr<Adapter> Adapter::Create(DXGI_GPU_PREFERENCE gpu_preference)
{
    using namespace Microsoft::WRL;

    std::shared_ptr<Adapter> adapter;

    ComPtr<IDXGIFactory6> dxgi_factory_6;
    ComPtr<IDXGIAdapter> dxgi_adapter;
    ComPtr<IDXGIAdapter4> dxgi_adapter_4;

    UINT create_factory_flags = 0;

#if defined (_DEBUG)
    create_factory_flags = DXGI_CREATE_FACTORY_DEBUG;
#endif

    for(UINT i = 0; dxgi_factory_6->EnumAdapterByGpuPreference(i, gpu_preference, IID_PPV_ARGS(&dxgi_adapter)) != DXGI_ERROR_NOT_FOUND; ++i)
    {
        if(SUCCEEDED(D3D12CreateDevice(dxgi_adapter.Get(), D3D_FEATURE_LEVEL_12_2, __uuidof(ID3D12Device), nullptr)))
        {
            throwIfFailed(dxgi_adapter.As(&dxgi_adapter_4));
            break;
        }
    }

    if(dxgi_adapter_4)
    {
        adapter = std::make_shared<MakeAdapter>(dxgi_adapter_4);
    }

    return adapter;
}

Adapter::Adapter(Microsoft::WRL::ComPtr<IDXGIAdapter4> dxgi_adapter)
    : adapter{dxgi_adapter}
    , adapter_descriptor{0}
{
    if(adapter)
    {
        throwIfFailed(adapter->GetDesc3(&adapter_descriptor));
    }
}

    MakeAdapter::MakeAdapter(Microsoft::WRL::ComPtr<IDXGIAdapter4> dxgi_adapter)
        : Adapter(dxgi_adapter)
    {}

    MakeAdapter::~MakeAdapter() {}

} // namespace gg;
