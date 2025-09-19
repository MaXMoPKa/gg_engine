module;

#include <mutex>
#include <memory>
#include <vector>
#include <unordered_map>
#include <map>

#include <d3d12.h>

export module render:resource_state_tracker;

namespace gg
{

class CommandList;

export class ResourceStateTracker
{
public:
    static void lock();
    static void unlock();

    uint32_t flushPendingResourceBarriers(const std::shared_ptr<CommandList>& command_list);

    void flushResourceBarriers(const std::shared_ptr<CommandList>& command_list);

    void commitFinalResourceStates();

    void reset();

protected:

private:
    using ResourceBarriers = std::vector<D3D12_RESOURCE_BARRIER>;

    ResourceBarriers pending_resource_barriers;
    ResourceBarriers resource_barriers;

    struct ResourceState
    {
    public:
        explicit ResourceState(D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON)
            : state{state}
        {}

        inline void setSubresourceState(UINT subresource, D3D12_RESOURCE_STATES state)
        {
            if(subresource == D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES)
            {
                this->state = state;
                subresource_state.clear();
            }
            else
            {
                subresource_state[subresource] = state;
            }
        }

        inline D3D12_RESOURCE_STATES getSubresourceState(UINT subresource)
        {
            D3D12_RESOURCE_STATES state = this->state;
            const auto iter = subresource_state.find(subresource);
            if(iter != subresource_state.end())
            {
                state = iter->second;
            }

            return state;

        }

        D3D12_RESOURCE_STATES state;
        std::map<UINT, D3D12_RESOURCE_STATES> subresource_state;
    };

    using ResourceList = std::vector<ID3D12Resource*>;
    using ResourceStateMap = std::unordered_map<ID3D12Resource*, ResourceState>;

    ResourceStateMap final_resource_state;
    static ResourceStateMap global_resource_state;

    static std::mutex global_mutex;
    static bool       is_locked;

};

} // namespace gg;
