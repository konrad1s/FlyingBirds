#pragma once

#include <cstdint>
#include <vector>
#include "EntityType.h"

namespace ConfigServer
{
    struct EntitySpawnConfig
    {
        EntityType type;
        float spawnInterval;
        int minCount;
    };

    constexpr int port = 5000;

    constexpr float xWorldSize = 720.0f;
    constexpr float xWorldMargin = 20.0f;
    constexpr float yWorldSize = 720.0f;
    constexpr float yWorldMargin = 20.0f;

    const std::vector<EntitySpawnConfig> entitySpawnConfigs =
    {
        {EntityType::Food,       5.f,  10},
        {EntityType::SpeedBoost, 10.f, 3},
        {EntityType::Protection, 15.f, 2}
    };
}
