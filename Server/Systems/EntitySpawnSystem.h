#pragma once

#include "ISystem.h"
#include "ConfigServer.h"

class EntitySpawnSystem : public ISystem
{
public:
    EntitySpawnSystem();
    void update(GameWorld &world, float deltaTime) override;

private:
    struct SpawnTracker
    {
        const ConfigServer::EntitySpawnConfig &config;
        float elapsedTime;

        SpawnTracker(const ConfigServer::EntitySpawnConfig &cfg)
            : config(cfg), elapsedTime(0.f) {}
    };

    std::vector<SpawnTracker> spawnTrackers;
};
