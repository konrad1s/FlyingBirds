#include "CollisionSystem.h"
#include "Logger.h"
#include <vector>
#include <cmath>

void CollisionSystem::update(GameWorld &world, float deltaTime)
{
    /**
     * TODO: naive approach, fix later
     */

    std::vector<std::size_t> foodsToErase;
    foodsToErase.reserve(world.getFood().size());

    for (auto& [playerId, playerPtr] : world.getPlayers())
    {
        float px = playerPtr->getX();
        float py = playerPtr->getY();

        float pRadius = playerPtr->getRadius();

        for (std::size_t i = 0; i < world.getFood().size(); i++)
        {
            const auto &food = world.getFood()[i];

            float fx = food.getX();
            float fy = food.getY();
            float fRadius = food.getRadius();

            float dx = px - fx;
            float dy = py - fy;
            float distSq = dx * dx + dy * dy;

            float radiusSum = pRadius + fRadius;

            if (distSq <= radiusSum * radiusSum)
            {
                playerPtr->addMass(food.getMass());

                foodsToErase.push_back(i);
            }
        }
    }

    std::sort(foodsToErase.begin(), foodsToErase.end());
    foodsToErase.erase(std::unique(foodsToErase.begin(), foodsToErase.end()), foodsToErase.end());

    for (auto it = foodsToErase.rbegin(); it != foodsToErase.rend(); ++it)
    {
        if (*it < world.getFood().size())
        {
            world.removeFoodAt(*it);
        }
    }
}
