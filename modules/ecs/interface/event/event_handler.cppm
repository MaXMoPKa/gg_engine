module;

#include <utility>

export module ecs.event_handler;

import ecs.global_memory_user;
import ecs.ievent_dispatcher;
import ecs.ievent;
import ecs.linear_allocator;
import ecs.ievent_delegate;
import ecs.event_dispatcher;

import types.unordered_map;
import types.vector;
import types.pair;

namespace gg
{
    export class EventHandler : private memory::GlobalMemoryUser
    {
        private:
            using EventDispatcherMap = UnorderedMap<EventTypeId, IEventDispatcher*>;
            using EventStorage = Vector<IEvent*>;
            using EventMemoryAllocator = memory::allocator::LinearAllocator;
        public:
            EventHandler();
            ~EventHandler();

            inline void clearEventBuffer()
            {
                this->getEventMemoryAllocator()->clear();
                this->getEventStorage().clear();
            }

            inline void clearEventDispatcher() { this->getEventDispatcherMap().clear(); }

            template<typename E, typename... ARGS>
            void send(ARGS&... args)
            {
                void* memory_ptr = this->getEventMemoryAllocator()->allocate(sizeof(E), alignof(E));
                if(memory_ptr != nullptr)
                {
                    this->getEventStorage().push_back(new (memory_ptr) E(std::forward<ARGS>(args)...));
                    // LogTrace("%s event buffered.", typeid(E).name());
                }
                else
                {
                    // LogWarning("Event buffer is full! Cut off new incoming events !!!");
                }
            }

            void dispatchEvents();

        private:
            inline void setEventDispatcherMap(const EventDispatcherMap& dispatchers)
            {
                this->dispatchers = dispatchers;
            }
            inline const EventDispatcherMap& getEventDispatcherMap() const { return this->dispatchers; }
            inline EventDispatcherMap& getEventDispatcherMap() { return this->dispatchers; }

            inline void setEventMemoryAllocator(EventMemoryAllocator* event_memory_allocator)
            {
                this->allocator = event_memory_allocator;
            }
            inline const EventMemoryAllocator* getEventMemoryAllocator() const { return this->allocator; }
            inline EventMemoryAllocator* getEventMemoryAllocator() { return this->allocator; }

            inline void setEventStorage(const EventStorage& event_storage) { this->events = event_storage; }
            inline const EventStorage& getEventStorage() const { return this->events; }
            inline EventStorage& getEventStorage() { return this->events; }

            template<typename E>
            inline void addEventCallback(IEventDelegate* const delegate)
            {
                EventTypeId ETID = E::STATIC_EVENT_TYPE_ID;

                EventDispatcherMap::const_iterator iter = this->getEventDispatcherMap().find(ETID);
                if(iter == this->getEventDispatcherMap().end())
                {
                    Pair<EventTypeId, IEventDispatcher*> kvp(ETID, new EventDispatcher<E>());
                    kvp.second->addEventCallback(delegate);
                    this->getEventDispatcherMap().insert(kvp);
                }
                else
                {
                    this->getEventDispatcherMap()[ETID]->addEventCallback(delegate);
                }
            }

            inline void removeEventCallback(IEventDelegate* delegate)
            {
                EventTypeId type_id = delegate->getStaticEventTypeId();
                EventDispatcherMap::const_iterator iter = this->getEventDispatcherMap().find(type_id);
                if(iter != this->getEventDispatcherMap().end())
                {
                    this->getEventDispatcherMap()[type_id]->removeEventCallback(delegate);
                }
            }

        private:
            EventDispatcherMap dispatchers;
            EventMemoryAllocator* allocator;
            EventStorage events;
    }; // class EventHandler;
} // namespace gg;
