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
    constexpr float worldSize = 800.0f;

    const std::vector<EntitySpawnConfig> entitySpawnConfigs =
        {
            {EntityType::Food, 5.f, 10},
            {EntityType::SpeedBoost, 10.f, 3}
        };
}
