#include "EntitySpawnSystem.h"
#include "GameWorld.h"
#include "Food.h"
#include "SpeedBoost.h"
#include "Protection.h"
#include "Logger.h"
#include "ConfigServer.h"
#include <random>

EntitySpawnSystem::EntitySpawnSystem(GameWorld &world)
{
    spawnTrackers.reserve(ConfigServer::entitySpawnConfigs.size());
    for (const auto &cfg : ConfigServer::entitySpawnConfigs)
    {
        spawnTrackers.emplace_back(cfg);
    }

    initialSpawning(world);
}

float EntitySpawnSystem::randomFloat(float minVal, float maxVal, std::mt19937 &rng)
{
    std::uniform_real_distribution<float> dist(minVal, maxVal);
    return dist(rng);
}

float EntitySpawnSystem::getRandomSpawnInterval(const ConfigServer::EntitySpawnConfig &cfg, std::mt19937 &rng)
{
    return randomFloat(cfg.minSpawnInterval, cfg.maxSpawnInterval, rng);
}

void EntitySpawnSystem::initialSpawning(GameWorld &world)
{
    for (auto &tracker : spawnTrackers)
    {
        int currentCount = 0;
        for (const auto &entityPtr : world.getEntities())
        {
            if (entityPtr->getEntityType() == tracker.config.type)
            {
                currentCount++;
            }
        }
        if (currentCount < tracker.config.minCount)
        {
            int toSpawn = tracker.config.minCount - currentCount;
            spawnEntities(world, tracker.config, toSpawn);
        }
    }
}

void EntitySpawnSystem::update(GameWorld &world, float deltaTime)
{
    static uint32_t entityIdCounter = 0;

    auto &rng = world.getRNG();

    for (auto &tracker : spawnTrackers)
    {
        if (tracker.currentSpawnInterval <= 0.0f)
        {
            tracker.currentSpawnInterval = getRandomSpawnInterval(tracker.config, rng);
        }

        tracker.elapsedTime += deltaTime;

        if (tracker.elapsedTime >= tracker.currentSpawnInterval)
        {
            tracker.elapsedTime = 0.0f;
            tracker.currentSpawnInterval = getRandomSpawnInterval(tracker.config, rng);

            int currentCount = 0;
            for (const auto &entityPtr : world.getEntities())
            {
                if (entityPtr->getEntityType() == tracker.config.type)
                    currentCount++;
            }

            if (currentCount < tracker.config.minCount)
            {
                int toSpawn = tracker.config.minCount - currentCount;

                float extraMax = 0.25f * tracker.config.minCount;
                int extra = static_cast<int>(randomFloat(0.0f, extraMax, rng));

                toSpawn += extra;

                spawnEntities(world, tracker.config, toSpawn);
            }
        }
    }
}

void EntitySpawnSystem::spawnEntities(GameWorld &world,
                                      const ConfigServer::EntitySpawnConfig &cfg,
                                      int count)
{
    static uint32_t uniqueIdGenerator = 0;

    auto &rng = world.getRNG();

    std::uniform_real_distribution<float> xPosDist(ConfigServer::xWorldMargin,
                                                   ConfigServer::xWorldSize - ConfigServer::xWorldMargin);
    std::uniform_real_distribution<float> yPosDist(ConfigServer::yWorldMargin,
                                                   ConfigServer::yWorldSize - ConfigServer::yWorldMargin);

    std::uniform_real_distribution<float> massDist(300.f, 600.f);

    for (int i = 0; i < count; ++i)
    {
        uint32_t newId = uniqueIdGenerator++;

        float x = xPosDist(rng);
        float y = yPosDist(rng);

        std::unique_ptr<Entity> newEntity;

        switch (cfg.type)
        {
        case EntityType::Food:
        {
            float mass = massDist(rng);
            newEntity = std::make_unique<Food>(newId, x, y, mass);
            break;
        }
        case EntityType::SpeedBoost:
        {
            newEntity = std::make_unique<SpeedBoost>(newId, x, y);
            break;
        }
        case EntityType::Protection:
        {
            newEntity = std::make_unique<Protection>(newId, x, y);
            break;
        }
        default:
            Logger::warning("EntitySpawnSystem: Unknown EntityType {}", static_cast<int>(cfg.type));
            continue;
        }

        world.addEntity(std::move(newEntity));
    }
}
