module;

#include <typeinfo>

export module ecs.event;

import ecs.ievent;

namespace gg
{
    export template<typename T>
    class Event : public IEvent
    {
        public:
            static const EventTypeId STATIC_EVENT_TYPE_ID;
            Event()
                : IEvent(STATIC_EVENT_TYPE_ID)
            {}
    }; // class Event;

    template<typename T>
    const EventTypeId Event<T>::STATIC_EVENT_TYPE_ID{typeid(T).hash_code()};
} // namespace gg;
