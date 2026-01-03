module;

export module ecs.ievent_delegate;

import types.base_types;

namespace gg
{
    class IEvent;
    
    export using EventDelegateId = Size;

    export class IEventDelegate
    {
        public:
            IEventDelegate() = default;
            virtual ~IEventDelegate() = default;

            virtual IEventDelegate* clone() = 0;
            virtual void invoke(const IEvent* const e) = 0;
            virtual EventDelegateId getDelegateId() const = 0;
            virtual U64 getStaticEventTypeId() const = 0;
            virtual Bool operator==(const IEventDelegate* other) const = 0;
    }; // class IEventDelegate;
} // namespace gg;
