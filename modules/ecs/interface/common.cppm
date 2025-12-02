module;

#include <cstdint>
#include <limits>

export module ecs:common;

import :platform;
import :handle;

namespace gg
{
    export const uint32_t ENITY_LUT_GROW {1024};
    export const uint32_t ENTITY_T_CHUNK_SIZE {512};
    export const uint32_t COMPONENT_LUT_GROW {1024};
    export const uint32_t COMPONENT_T_CHUNK_SIZE {512};
    export const uint32_t ECS_EVENT_MEMORY_BUFFER_SIZE {4194304}; // 4MB
    export const uint32_t ECS_SYSTEM_MEMORY_BUFFER_SIZE {8388608}; // 8MB
    export const uint32_t ECS_GLOBAL_MEMORY_CAPACITY {134217728}; // 128 MB

    export using EntityId = util::Handle64;
    export using EntityTypeId = TypeId;

    export const EntityId INVALID_ENTITY_ID = util::Handle64::INVALID_HANDLE;

    export using ComponentId = ObjectId;
    export using ComponentTypeId = TypeId;

    export const ComponentId INVALID_COMPONENT_ID = INVALID_OBJECT_ID;

    export using SystemTypeId = TypeId;
    export using SystemPriority = uint16_t;

    export const SystemTypeId INVALID_SYSTEM_ID = INVALID_TYPE_ID;
    export const SystemPriority LOWEST_SYSTEM_PRIORITY = std::numeric_limits<SystemPriority>::min();
    export const SystemPriority VERY_LOW_SYSTEM_PRIORITY = 99u;
    export const SystemPriority LOW_SYSTEM_PRIORITY = 100u;
    export const SystemPriority NORMAL_SYSTEM_PRIORITY = 200u;
    export const SystemPriority MEDIUM_SYSTEM_PRIORITY = 300u;
    export const SystemPriority HIGH_SYSTEM_PRIORITY = 400u;
    export const SystemPriority VERY_HIGH_SYSTEM_PRIORITY = 500u;
    export const SystemPriority HIGHEST_SYSTEM_PRIORITY = std::numeric_limits<SystemPriority>::max();
} // namespace gg;
