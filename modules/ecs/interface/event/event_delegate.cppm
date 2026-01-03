module;

#include <typeinfo>

export module ecs.event_delegate;

import ecs.ievent_delegate;

import types.base_types;

namespace gg
{
    class IEvent;
    
    export template<typename C, typename E>
    class EventDelegate : public IEventDelegate
    {
        private:
            using Callback = void (C::*)(const E* const);

        public:
            EventDelegate(C* receiver, Callback&  callback_function)
                : receiver(receiver)
                , callback(callback_function)
            {}

            virtual IEventDelegate* clone() override { return new EventDelegate(this->receiver, this->callback); }

            virtual void invoke(const IEvent* const e)
            {
                (this->receiver->*callback)(reinterpret_cast<const E* const>(e));
            }
            
            virtual EventDelegateId getDelegateId() const override
            {
                static const EventDelegateId DELEGATE_ID{ typeid(C).hash_code() ^ typeid(Callback).hash_code() };
                return DELEGATE_ID;
            }

            virtual U64 getStaticEventTypeId() const override
            {
                static const U64 SEID{E::STATIC_EVENT_TYPE_ID};
                return SEID;
            }

            virtual Bool operator==(const IEventDelegate* other) const override
            {
                if(this->getDelegateId() != other->getDelegateId())
                {
                    return false;
                }

                EventDelegate* delegate = (EventDelegate*)other;
                if(other == nullptr)
                {
                    return false;
                }

                return ((this->callback == delegate->callback) && (this->receiver == delegate->receiver));
            }

        private:
            inline void setReceiver(C* receiver) { this->receiver = receiver; }
            inline C* getReceiver() { return this->receiver; }
            inline const C* getReceiver() const { return this->receiver; }

            inline void setCallback(const Callback& callback) { this->callback = callback; }
            inline Callback& getCallback() { return this->callback; }
            inline const Callback& getCallback() const { return this->callback; }
            
        private:
            C* receiver;
            Callback callback;
    }; // class EventDelegate;
} // namespace gg;
