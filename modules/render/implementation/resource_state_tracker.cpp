module;

#include <memory>
#include <mutex>
#include <cassert>

#include <d3d12.h>

#include <wrl.h>

module render;

import :resource_state_tracker;
import :command_list;

namespace gg
{
    std::mutex ResourceStateTracker::global_mutex;
    bool ResourceStateTracker::is_locked = false;
    ResourceStateTracker::ResourceStateMap ResourceStateTracker::global_resource_state;

    void ResourceStateTracker::lock()
    {
        global_mutex.lock();
        is_locked = true;
    }

    void ResourceStateTracker::unlock()
    {
        global_mutex.unlock();
        is_locked = false;
    }

    uint32_t ResourceStateTracker::flushPendingResourceBarriers(const std::shared_ptr<CommandList>& command_list)
    {
        assert(is_locked);
        assert(command_list);

        ResourceBarriers resource_barriers;

        resource_barriers.reserve(pending_resource_barriers.size());

        for(D3D12_RESOURCE_BARRIER pending_barrier: pending_resource_barriers)
        {
            if(pending_barrier.Type == D3D12_RESOURCE_BARRIER_TYPE_TRANSITION)
            {
                D3D12_RESOURCE_TRANSITION_BARRIER pending_transition = pending_barrier.Transition;

                const auto& iter = global_resource_state.find( pending_transition.pResource );
                if(iter != global_resource_state.end())
                {
                    ResourceState& resource_state = iter->second;
                    if(pending_transition.Subresource == D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES &&
                       !resource_state.subresource_state.empty())
                    {
                        for(auto [subresource, state]: resource_state.subresource_state)
                        {
                            if(pending_transition.StateAfter != state)
                            {
                                D3D12_RESOURCE_BARRIER new_barrier = pending_barrier;
                                new_barrier.Transition.Subresource = subresource;
                                new_barrier.Transition.StateBefore = state;
                                resource_barriers.push_back( new_barrier );
                            }
                        }
                    }
                    else
                    {
                        D3D12_RESOURCE_STATES global_state = (iter->second).getSubresourceState(pending_transition.Subresource);
                        if(pending_transition.StateAfter != global_state)
                        {
                            pending_barrier.Transition.StateBefore = global_state;
                            resource_barriers.push_back(pending_barrier);
                        }
                    }
                }
            }
        }

        UINT num_barriers = static_cast<UINT>(resource_barriers.size());
        if(num_barriers > 0)
        {
            Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> d3d12_command_list = command_list->getD3D12CommandList();
            d3d12_command_list->ResourceBarrier( num_barriers, resource_barriers.data() );
        }

        pending_resource_barriers.clear();

        return num_barriers;
    }

    void ResourceStateTracker::flushResourceBarriers(const std::shared_ptr<CommandList>& command_list)
    {
        assert(command_list);

        UINT num_barriers = static_cast<UINT>(resource_barriers.size());
        if(num_barriers > 0)
        {
            Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> d3d12_command_list = command_list->getD3D12CommandList();
            d3d12_command_list->ResourceBarrier(num_barriers, resource_barriers.data());
            resource_barriers.clear();
        }
    }

    void ResourceStateTracker::commitFinalResourceStates()
    {
        assert(is_locked);

        for(const auto& [resource, state] : final_resource_state)
        {
            global_resource_state[resource] = state;
        }

        final_resource_state.clear();
    }

    void ResourceStateTracker::reset()
    {
        pending_resource_barriers.clear();
        resource_barriers.clear();
        final_resource_state.clear();
    }
} // namespace gg;
