module;

module ecs.ievent;

namespace gg
{
    IEvent::IEvent(EventTypeId type_id)
        : type_id{type_id}
    {
        //assert(ecs_engine != nullptr && "ECS engine not initialized!");
        //this->time_created = ecs_engine->ecs_engine_time->getTimeStamp();
    }
} // namespace gg;
