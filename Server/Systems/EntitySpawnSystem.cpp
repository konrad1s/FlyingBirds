#include "EntitySpawnSystem.h"
#include "GameWorld.h"
#include "Food.h"
#include "SpeedBoost.h"
#include "Protection.h"
#include "Logger.h"
#include "ConfigServer.h"
#include <random>

EntitySpawnSystem::EntitySpawnSystem()
{
    for (const auto &cfg : ConfigServer::entitySpawnConfigs)
    {
        spawnTrackers.emplace_back(cfg);
    }
}

void EntitySpawnSystem::update(GameWorld &world, float deltaTime)
{
    static uint32_t entityId = 0;

    for (auto &tracker : spawnTrackers)
    {
        tracker.elapsedTime += deltaTime;

        if (tracker.elapsedTime < tracker.config.spawnInterval)
            continue;

        tracker.elapsedTime = 0.f;

        int currentCount = 0;
        for (const auto &entityPtr : world.getEntities())
        {
            if (entityPtr->getEntityType() == tracker.config.type)
                currentCount++;
        }

        if (currentCount < tracker.config.minCount)
        {
            int toSpawn = tracker.config.minCount - currentCount;

            std::uniform_real_distribution<float> xPos(ConfigServer::xWorldMargin,
                                                       ConfigServer::xWorldSize - ConfigServer::xWorldMargin);
            std::uniform_real_distribution<float> yPos(ConfigServer::yWorldMargin,
                                                       ConfigServer::yWorldSize - ConfigServer::yWorldMargin);
            std::uniform_real_distribution<float> massDist(300.f, 600.f);
            std::mt19937 &rng = world.getRNG();

            for (int i = 0; i < toSpawn; ++i)
            {
                uint32_t uniqueId = entityId++;
                float x = xPos(rng);
                float y = yPos(rng);
                float mass = 0.f;

                std::unique_ptr<Entity> newEntity;

                switch (tracker.config.type)
                {
                case EntityType::Food:
                    mass = massDist(rng);
                    newEntity = std::make_unique<Food>(uniqueId, x, y, mass);
                    break;
                case EntityType::SpeedBoost:
                    newEntity = std::make_unique<SpeedBoost>(uniqueId, x, y);
                    break;
                case EntityType::Protection:
                    newEntity = std::make_unique<Protection>(uniqueId, x, y, 5.f);
                    break;
                default:
                    Logger::warning("EntitySpawnSystem: Unknown EntityType {}", static_cast<int>(tracker.config.type));
                    continue;
                }

                world.addEntity(std::move(newEntity));
            }
        }
    }
}
