module;

#include <algorithm>

export module ecs.event_dispatcher;

import ecs.ievent_dispatcher;
import ecs.ievent_delegate;
import ecs.ievent;

import types.list;
import types.base_types;

namespace gg
{    
    export template<typename T>
    class EventDispatcher : public IEventDispatcher
    {
        private:
            using EventDelegateList = List<IEventDelegate*>;
            using PendingRemoveDelegates = List<IEventDelegate*>;

        public:
            EventDispatcher()
                : locked(false)
            {}
            virtual ~EventDispatcher() override
            {
                this->pending_remove_delegates.clear();
                this->event_callbacks.clear();
            }
            
            virtual void dispatch(IEvent* event) override
            {
                this->setLocked(true);
                // LogTrace("Dispatch event %s", typeid(T).name());
                if(this-getPendingRemoveDelegates().empty() == false)
                {
                    for(auto EC : this->getPendingRemoveDelegates())
                    {
                        auto result = std::find_if(this->getEventCallbacks().begin(), this->getEventCallbacks().end(), [&](const IEventDelegate* other){ return other == EC; });
                        if(result != this->getEventCallbacks().end())
                        {
                            IEventDelegate* delegate = (IEventDelegate*)(*result);
                            this->getEventCallbacks().erase(result);
                            delete delegate;
                            delegate = nullptr;
                        }
                    }
                    this->getPendingRemoveDelegates().clear();
                }

                for(auto EC : this->getEventCallbacks())
                {
                    assert(EC != nullptr && "Invalid event callback.");
                    EC->invoke(event);
                }

                this->setLocked(false);
            }

            virtual void addEventCallback(IEventDelegate* event_delegate) override
            {
                auto result = std::find_if(this->getPendingRemoveDelegates().begin(), this->getPendingRemoveDelegates().end(), [&](const IEventDelegate* other) { return other == event_delegate; });
                if(result != this->getPendingRemoveDelegates().end())
                {
                    this->getPendingRemoveDelegates().erase(result);
                    return;
                }

                this->getEventCallbacks().push_back(event_delegate);
            }

            virtual void removeEventCallback(IEventDelegate* event_delegate) override
            {
                if(!this->getLocked())
                {
                    auto result = std::find_if(this->getEventCallbacks().begin(), this->getEventCallbacks().end(), [&](const IEventDelegate* other) { return other == event_delegate; });
                    if(result != this->getEventCallbacks().end())
                    {
                        IEventDelegate* delegate = (IEventDelegate*)(*result);
                        this->getEventCallbacks().erase(result);
                        delete delegate;
                        delegate = nullptr;
                    }
                }
                else
                {
                    auto result = std::find_if(this->getEventCallbacks().begin(), this->getEventCallbacks().end(), [&](const IEventDelegate* other) { return other == event_delegate; });

                    if(result != this->getEventCallbacks().end())
                    {
                        this->getPendingRemoveDelegates().push_back((*result));
                    }
                }
            }
            virtual Size getEventCallbackCount() const override { return this->getEventCallbacks().size(); }
            
        private:
            inline void setPendingRemoveDelegates(const PendingRemoveDelegates& pending_remove_delegates)
            {
                this->pending_remove_delegates = pending_remove_delegates;
            }
            inline const PendingRemoveDelegates& getPendingRemoveDelegates() const { return this->pending_remove_delegates; }
            inline PendingRemoveDelegates& getPendingRemoveDelegates() { return this->pending_remove_delegates; }

            inline void setEventCallbacks(const EventDelegateList& event_callbacks) { this->event_callbacks = event_callbacks; }
            inline const EventDelegateList& getEventCallbacks() const { return this->event_callbacks; }
            inline EventDelegateList& getEventCallbacks() { return this->event_callbacks; }

            inline void setLocked(Bool locked) { this->locked = locked; }
            inline Bool getLocked() const {return this->locked; }
            inline Bool getLocked() {return this->locked; }
            
        private:
            PendingRemoveDelegates pending_remove_delegates;
            EventDelegateList event_callbacks;
            Bool locked;
    }; // class EventDispatcher;
} // namespace gg;
