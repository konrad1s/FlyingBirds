#pragma once

#include <cstdint>
#include "EntityType.h"

namespace ConfigServer
{
    struct EntitySpawnConfig
    {
        EntityType type;
        float minSpawnInterval;
        float maxSpawnInterval;
        int minCount;
    };

    constexpr int port = 12345;

    constexpr float xWorldSize = 720.0f;
    constexpr float xWorldMargin = 20.0f;
    constexpr float yWorldSize = 720.0f;
    constexpr float yWorldMargin = 20.0f;

    constexpr float playerCollisionAttackFactor = 0.001f;

    const std::vector<EntitySpawnConfig> entitySpawnConfigs =
    {
        {EntityType::Food,       3.f,  5.f, 10},
        {EntityType::SpeedBoost, 8.f, 10.f,  3},
        {EntityType::Protection,12.f, 15.f,  2}
    };
}
