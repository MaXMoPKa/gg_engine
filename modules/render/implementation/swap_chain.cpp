module;

#define NOMINMAX

#include <string>
#include <cassert>
#include <memory>

#include <d3d12.h>
#include <dxgiformat.h>
#include <dxgi1_6.h>

#include <windows.h>
#include <wrl.h>

module render;

import :swap_chain;
import :device;
import :adapter;

namespace gg
{

SwapChain::SwapChain(Device& device, HWND window_handle, DXGI_FORMAT render_target_format)
    : device{device}
    , command_queue{this->device.getCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT)}
    , window_handle{window_handle}
    , fence_values{0}
    , width {0u}
    , height {0u}
    , d3d12_render_target_format{render_target_format}
    , v_sync{true}
    , tearing_supported{false}
    , fullscreen{false}
{
    assert(window_handle);

    Microsoft::WRL::ComPtr<ID3D12CommandQueue> d3d12_command_queue = this->command_queue.getD3D12CommandQueue();

    std::shared_ptr<Adapter> adapter = this->device.getAdapter();
    Microsoft::WRL::ComPtr<IDXGIAdapter4> dxgi_adapter = adapter->getDXGIAdapter();

    Microsoft::WRL::ComPtr<IDXGIFactory> dxgi_factory;
    Microsoft::WRL::ComPtr<IDXGIFactory5> dxgi_factory_5;
    throwIfFailed(dxgi_adapter->GetParent(IID_PPV_ARGS(&dxgi_factory)));
    throwIfFailed(dxgi_factory.As(&dxgi_factory_5));

    BOOL allow_tearing = FALSE;
    if(SUCCEEDED(dxgi_factory_5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allow_tearing, sizeof(BOOL))))
    {
        this->tearing_supported = (allow_tearing == TRUE);
    }

    RECT window_rect;
    ::GetClientRect(window_handle, &window_rect);

    this->width = window_rect.right - window_rect.left;
    this->height = window_rect.bottom - window_rect.top;

    DXGI_SWAP_CHAIN_DESC1 swap_chain_desc = {};
    swap_chain_desc.Width = this->width;
    swap_chain_desc.Height = this->height;
    swap_chain_desc.Format = this->d3d12_render_target_format;
    swap_chain_desc.Stereo = FALSE;
    swap_chain_desc.SampleDesc = {1, 0};
    swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_desc.BufferCount = this->buffer_count;
    swap_chain_desc.Scaling = DXGI_SCALING_STRETCH;
    swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swap_chain_desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    swap_chain_desc.Flags = this->tearing_supported ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;
    swap_chain_desc.Flags |= DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;

    Microsoft::WRL::ComPtr<IDXGISwapChain1> dxgi_swap_chain_1;
    throwIfFailed(dxgi_factory_5->CreateSwapChainForHwnd(d3d12_command_queue.Get(), this->window_handle, &swap_chain_desc, nullptr, nullptr, &dxgi_swap_chain_1));

    throwIfFailed(dxgi_swap_chain_1.As(&this->d3d12_swap_chain));

    throwIfFailed(dxgi_factory_5->MakeWindowAssociation(this->window_handle, DXGI_MWA_NO_ALT_ENTER));

    this->current_back_buffer_index = this->d3d12_swap_chain->GetCurrentBackBufferIndex();

    this->d3d12_swap_chain->SetMaximumFrameLatency(this->buffer_count - 1);

    this->frame_latency_waitable_objects_handle = this->d3d12_swap_chain->GetFrameLatencyWaitableObject();

    this->updateRenderTargetViews();
}

SwapChain::~SwapChain()
{

}

void SwapChain::setFullscreen(bool fullscreen)
{
    if(this->fullscreen != fullscreen)
    {
        this->fullscreen = fullscreen;
    }
}

void SwapChain::waitForSwapChain()
{
    DWORD result = ::WaitForSingleObjectEx(this->frame_latency_waitable_objects_handle, 1000, TRUE);
}

void SwapChain::resize(uint32_t width, uint32_t height)
{
    if(this->width != width || this->height != height)
    {
        this->width = std::max(1u, width);
        this->height = std::max(1u, height);

        this->device.flush();

        this->render_target.reset();
        for(UINT i = 0; i < this->buffer_count; ++i)
        {
            this->back_buffer_textures[i].reset();
        }

        DXGI_SWAP_CHAIN_DESC swap_chain_desc = {};
        throwIfFailed(this->d3d12_swap_chain->GetDesc(&swap_chain_desc));
        throwIfFailed(this->d3d12_swap_chain->ResizeBuffers(this->buffer_count, this->width, this->height, swap_chain_desc.BufferDesc.Format, swap_chain_desc.Flags));

        this->current_back_buffer_index = this->d3d12_swap_chain->GetCurrentBackBufferIndex();

        this->updateRenderTargetViews();
    }
}

const RenderTarget& SwapChain::getRenderTarget() const
{
    this->render_target.attachTexture(AttachmentPoint::COLOR_0, this->back_buffer_textures[this->current_back_buffer_index]);
    return this->render_target;
}

UINT SwapChain::present(const std::shared_ptr<Texture>& texture)
{
    std::shared_ptr<CommandList> command_list = this->command_queue.getCommandList();
    std::shared_ptr<Texture> back_buffer = this->back_buffer_textures[this->current_back_buffer_index];

    if(texture)
    {
        if(texture->getD3D12ResourceDesc().SampleDesc.Count > 1)
        {
            command_list->resolveSubresource(back_buffer, texture);
        }
        else
        {
            command_list->copyResource(back_buffer, texture);
        }
    }

    command_list->transitionBarrier(back_buffer, D3D12_RESOURCE_STATE_PRESENT);
    this->command_queue.executeCommandList(command_list);

    UINT sync_interval = this->v_sync ? 1 : 0;
    UINT present_flags = this->tearing_supported && !this->fullscreen && !this->v_sync ? DXGI_PRESENT_ALLOW_TEARING : 0;
    throwIfFailed(this->d3d12_swap_chain->Present(sync_interval, present_flags));

    this->fence_values[this->current_back_buffer_index] = this->command_queue.signal();

    this->current_back_buffer_index = this->d3d12_swap_chain->GetCurrentBackBufferIndex();

    UINT64 fence_value = this->fence_values[this->current_back_buffer_index];
    this->command_queue.waitForFenceValue(fence_value);

    this->device.releaseStaleDescriptors();

    return this->current_back_buffer_index;
}

void SwapChain::updateRenderTargetViews()
{
    for(UINT i = 0; i < this->buffer_count; ++i)
    {
        Microsoft::WRL::ComPtr<ID3D12Resource> back_buffer;
        throwIfFailed(this->d3d12_swap_chain->GetBuffer(i, IID_PPV_ARGS(&back_buffer)));

        ResourceStateTracker::addGlobalResourceState(back_buffer.Get(), D3D12_RESOURCE_STATE_COMMON);

        this->back_buffer_textures[i] = this->device.createTexture(back_buffer);

        this->back_buffer_textures[i]->setName(L"Back Buffer[" + std::to_wstring(i) + L"]");
    }
}

MakeSwapChain::MakeSwapChain(Device& device, HWND window_handle, DXGI_FORMAT back_buffer_format)
    : SwapChain(device, window_handle, back_buffer_format)
{}

MakeSwapChain::~MakeSwapChain() {}

} // namespace gg;
