module;

#include <string>
#include <cstdint>

export module ecs:isystem;

import :common;

namespace gg
{

export class ISystem
{
public:
    virtual ~ISystem() = default;

    [[nodiscard]] virtual const SystemTypeId getStaticSystemTypeId() const = 0;
    [[nodiscard]] virtual std::string getSystemTypeName() const = 0;

    virtual void preUpdate(float dt) = 0;
    virtual void update(float dt) = 0;
    virtual void postUpdate(float dt) = 0;

protected:
    ISystem(SystemPriority priority = NORMAL_SYSTEM_PRIORITY, float update_interval_ms = -1.0f)
        : system_priority{priority}
        , update_interval{update_interval_ms}
        , time_since_last_update{}
        , is_enabled{true}
        , is_needs_update{}
        , reserved{}
    {}

private:
    friend class SystemManager;

    float time_since_last_update;
    SystemPriority system_priority;
    float update_interval;
    uint8_t is_enabled : 1;
    uint8_t is_needs_update : 1;
    uint8_t reserved : 6;
};

} // namespace gg;
