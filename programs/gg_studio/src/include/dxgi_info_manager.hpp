#pragma once

#include <vector>
#include <string>

#include <dxgidebug.h>
#include <dxgi1_6.h>

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
            ~DXGIInfoManager();

            void set() noexcept;
            std::vector<std::string> getMessages() const;
        private:
            unsigned long long next = 0u;
            IDXGIInfoQueue* p_dxgi_info_queue = nullptr;
        };

    }

}
