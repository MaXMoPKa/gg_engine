module;

export module ecs.ievent;

import ecs.platform;

namespace gg
{
export using EventTypeId = TypeId;
export using EventTimeStamp = TimeStamp;

export const EventTypeId INVALID_EVENT_TYPE = INVALID_TYPE_ID;
    
export class IEvent
{
public:
    IEvent(EventTypeId event_type_id);
    virtual ~IEvent() = default;

    inline const EventTypeId& getTypeId() const { return this->type_id; }
    inline EventTypeId& getTypeId() { return this->type_id; }

    inline const EventTimeStamp& getTimeCreated() const { return this->time_created; }
    inline EventTimeStamp& getTimeCreated() { return this->time_created; }
private:
    inline void setTypeId(const EventTypeId& new_type_id) { this->type_id = new_type_id; }

    inline void setTimeCreated(const EventTimeStamp& new_time_created) {this->time_created = new_time_created; }
private:
    EventTypeId type_id;
    EventTimeStamp time_created;
}; // class IEvent;
} // namespace gg;
