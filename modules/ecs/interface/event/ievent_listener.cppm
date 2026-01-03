module;

export module ecs.ievent_listener;

import ecs.ievent_delegate;
import ecs.event_delegate;

import types.list;

namespace gg
{    
    export class IEventListener
    {
        using RegisteredCallbacks = List<IEventDelegate*>;
        public:
            IEventListener() = default;
            virtual ~IEventListener();

            template<typename E, typename C>
            inline void RegisterEventCallback(void (C::*Callback)(const E* const))
            {
                IEventDelegate* delegate = new EventDelegate<C, E>(static_cast<C*>(this), Callback);

                this->getRegisteredCallbacks().push_back(delegate);
                // ecs_engine->subscribeEvent<E>(delegate);
            }

            template<typename E, typename C>
            inline void unregisterEventCallback(void (C::*Callback)(const E* const))
            {
                EventDelegate<C, E> delegate(static_cast<C*>(this), Callback);

                for(auto cb : this->registered_callbacks)
                {
                    if(cb->getDelegateId() == delegate.getDelegateId())
                    {
                        this->getRegisteredCallbacks().remove_if([&](const IEventDelegate* other) { return other == cb; });
                        // ecs_engine->unsubscribe_event(&delegate);
                        break;
                    }
                }
            }

            void unregisterAllEventCallbacks();

        private:
            inline void setRegisteredCallbacks(const RegisteredCallbacks& registered_callbacks)
            {
                this->registered_callbacks = registered_callbacks;
            }

            inline const RegisteredCallbacks& getRegisteredCallbacks() const { return this->registered_callbacks; }
            inline RegisteredCallbacks& getRegisteredCallbacks() { return this->registered_callbacks; }
            
        private:
            RegisteredCallbacks registered_callbacks;
    }; // class IEventListener;
} // namespace gg;
