module;

module ecs.event_handler;

import ecs.common;

import types.base_types;

namespace gg
{
    EventHandler::EventHandler()
    {
        this->allocator = new EventMemoryAllocator(ECS_EVENT_MEMORY_BUFFER_SIZE, allocate(ECS_EVENT_MEMORY_BUFFER_SIZE, "EventHandler"));
        this->getEventStorage().reserve(1024);
    }

    EventHandler::~EventHandler()
    {
        for(auto& it : this->getEventDispatcherMap())
        {
            delete it.second;
            it.second = nullptr;
        }

        this->getEventDispatcherMap().clear();

        this->free((void*)this->getEventMemoryAllocator()->getMemoryAddress());

        delete this->getEventMemoryAllocator();
        this->setEventMemoryAllocator(nullptr);
    }

    void EventHandler::dispatchEvents()
    {
        Size last_index = this->getEventStorage().size();
        Size current_index = 0;

        while(current_index < last_index)
        {
            IEvent* event = this->getEventStorage()[current_index++];
            if(event == nullptr)
            {
                continue;
            }

            auto it = this->getEventDispatcherMap().find(event->getTypeId());
            if(it == this->getEventDispatcherMap().end())
            {
                continue;
            }

            it->second->dispatch(event);

            last_index = this->getEventStorage().size();
        }

        this->clearEventBuffer();
    }
} // namespace gg;
