module;

module ecs.ievent_listener;

namespace gg
{
    IEventListener::~IEventListener()
    {
        this->unregisterAllEventCallbacks();
    }

    void IEventListener::unregisterAllEventCallbacks()
    {
        for(auto cb : this->getRegisteredCallbacks())
        {
            // ecs_engine->unsubscribeEvent(cb);
        }

        this->getRegisteredCallbacks().clear();
    }
    
} // namespace gg;
