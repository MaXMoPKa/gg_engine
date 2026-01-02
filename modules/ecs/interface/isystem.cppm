module;

export module ecs.isystem;

import ecs.common;

import types.base_types;
import types.string;

namespace gg
{
    
export class ISystem
{
public:
    virtual ~ISystem() = default;

    [[nodiscard]] virtual const SystemTypeId getStaticSystemTypeId() const = 0;
    [[nodiscard]] virtual String getSystemTypeName() const = 0;

    virtual void preUpdate(F32 dt) = 0;
    virtual void update(F32 dt) = 0;
    virtual void postUpdate(F32 dt) = 0;

public:
    Bool isEnabled() const
    {
        return this->is_enabled;
    }
    void enable()
    {
        this->is_enabled = true;
    }
    void disable()
    {
        this->is_enabled = false;
    }

    [[nodiscard]] F32 getUpdateInterval() const
    {
        return this->update_interval;
    }
    void setUpdateInterval(F32 ms)
    {
        this->update_interval = ms;
    }

    [[nodiscard]] SystemPriority getPriority() const
    {
        return this->system_priority;
    }
    void setPriority(SystemPriority priority)
    {
        this->system_priority = priority;
    }
        
        
protected:
    ISystem(SystemPriority priority = NORMAL_SYSTEM_PRIORITY, F32 update_interval_ms = -1.0f)
        : system_priority{priority}
        , update_interval{update_interval_ms}
        , time_since_last_update{}
        , is_enabled{true}
        , is_needs_update{}
        , reserved{}
    {}

private:
    F32 time_since_last_update;
    SystemPriority system_priority;
    F32 update_interval;
    U8 is_enabled : 1;
    U8 is_needs_update : 1;
    U8 reserved : 6;
};

} // namespace gg;
