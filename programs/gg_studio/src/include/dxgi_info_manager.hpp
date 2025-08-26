#pragma once

#include <vector>
#include <string>

#include <dxgidebug.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <wrl/client.h>

namespace gg
{

    namespace gapi
    {

        class DXGIInfoManager
        {
        public:
            DXGIInfoManager();
            DXGIInfoManager(const DXGIInfoManager&) = delete;
            DXGIInfoManager& operator=(const DXGIInfoManager&) = delete;
            ~DXGIInfoManager() = default;

            void set() noexcept;
            std::vector<std::string> getMessages() const;
        private:
            unsigned long long next = 0u;
            Microsoft::WRL::ComPtr<IDXGIInfoQueue> p_dxgi_info_queue = nullptr;
        };

    }

}
