module;

#include <cstdint>

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

    export using EntityId = util::Handle64;
    export using EntityTypeId = TypeId;

    export const EntityId INVALID_ENTITY_ID = util::Handle64::INVALID_HANDLE;

    export using ComponentId = ObjectId;
    export using ComponentTypeId = TypeId;

    export const ComponentId INVALID_COMPONENT_ID = INVALID_OBJECT_ID;
} // namespace gg;
