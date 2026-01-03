module;

export module ecs.ievent_dispatcher;

import ecs.ievent;
import ecs.ievent_delegate;

import types.base_types;

namespace gg
{    
    export class IEventDispatcher
    {
        public:
            IEventDispatcher() = default;
            virtual ~IEventDispatcher() = default;

            virtual void addEventCallback(IEventDelegate* const event_delegate) = 0;
            virtual void removeEventCallback(IEventDelegate* event_delegate) = 0;
            virtual Size getEventCallbackCount() const = 0;

            virtual void dispatch(IEvent* event) = 0;
    }; // namespace IEventDispatcher;
} // namespace gg
