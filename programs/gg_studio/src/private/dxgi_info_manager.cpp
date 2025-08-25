#include "src/include/dxgi_info_manager.hpp"
#include "src/include/dx11_graphics.hpp"
#include "src/include/window.h"

#define GFX_THROW_NOINFO(hrcall) if(FAILED(hr == (hrcall))) throw gg::gapi::DX11Graphics::HrException(__LINE__, __FILE__, hr);

gg::gapi::DXGIInfoManager::DXGIInfoManager()
{

	HRESULT hr = DXGIGetDebugInterface1(0, __uuidof(IDXGIInfoQueue), reinterpret_cast<void**>(&p_dxgi_info_queue));

	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to get IDXGIInfoQueue interface");
	}
}

gg::gapi::DXGIInfoManager::~DXGIInfoManager()
{
	if (p_dxgi_info_queue != nullptr)
	{
		p_dxgi_info_queue->Release();
	}
}

void gg::gapi::DXGIInfoManager::set() noexcept
{
	next = p_dxgi_info_queue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::string> gg::gapi::DXGIInfoManager::getMessages() const
{
	std::vector<std::string> messages;
	const auto end = p_dxgi_info_queue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	for (auto i = next; i < end; i++)
	{
		HRESULT hr = 0;
		SIZE_T messageLength;
		GFX_THROW_NOINFO(p_dxgi_info_queue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength));
		auto bytes = std::make_unique<byte[]>(messageLength);
		auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
		GFX_THROW_NOINFO(p_dxgi_info_queue->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength));
		messages.emplace_back(pMessage->pDescription);
	}
	return messages;
}
