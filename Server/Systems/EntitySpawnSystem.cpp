#include "EntitySpawnSystem.h"
#include "GameWorld.h"
#include "Food.h"
#include "ConfigServer.h"
#include "Logger.h"
#include <random>

void EntitySpawnSystem::update(GameWorld &world, float deltaTime)
{
    static uint32_t id = 0;
    timeSinceLastSpawn += deltaTime;
    if (timeSinceLastSpawn < spawnInterval)
        return;

    timeSinceLastSpawn = 0.f;

    int foodCount = 0;
    for (auto &ePtr : world.getEntities())
    {
        if (ePtr->getEntityType() == EntityType::Food)
        {
            foodCount++;
        }
    }

    if (foodCount < ConfigServer::minFoods)
    {
        int toSpawn = ConfigServer::minFoods - foodCount;
        std::uniform_real_distribution<float> posDist(0.f, ConfigServer::worldSize);
        std::uniform_real_distribution<float> massDist(400.f, 1200.f);

        for (int i = 0; i < toSpawn; ++i)
        {
            uint32_t uniqueId = id++;
            float x = posDist(world.getRNG());
            float y = posDist(world.getRNG());
            float m = massDist(world.getRNG());

            auto food = std::make_unique<Food>(uniqueId, x, y, m);
            world.addEntity(std::move(food));
        }

        Logger::info("EntitySpawnSystem: spawned {} Food to reach min = {}", toSpawn, ConfigServer::minFoods);
    }
}
