#pragma once

#include <vector>
#include <memory>
#include <random>
#include "EntityType.h"
#include "ConfigServer.h"
#include "ISystem.h"

class GameWorld;
class Entity;

class EntitySpawnSystem  : public ISystem
{
public:
    EntitySpawnSystem(GameWorld& world);
    void update(GameWorld &world, float deltaTime);

private:
    void spawnEntities(GameWorld& world, const ConfigServer::EntitySpawnConfig& cfg, int count);
    static float getRandomSpawnInterval(const ConfigServer::EntitySpawnConfig& cfg, std::mt19937& rng);
    static float randomFloat(float minVal, float maxVal, std::mt19937& rng);
    void initialSpawning(GameWorld& world);

    struct SpawnTracker
    {
        SpawnTracker(const ConfigServer::EntitySpawnConfig& cfg)
            : config(cfg)
            , elapsedTime(0.0f)
            , currentSpawnInterval(0.0f)
        {}

        ConfigServer::EntitySpawnConfig config;
        float elapsedTime;
        float currentSpawnInterval;
    };

    std::vector<SpawnTracker> spawnTrackers;
};
