#include "CollisionSystem.h"
#include "GameWorld.h"
#include "Player.h"
#include "Food.h"
#include "SpeedBoost.h"
#include "Logger.h"
#include <cmath>
#include <vector>
#include <algorithm>

void CollisionSystem::update(GameWorld &world, float deltaTime)
{
    auto &entities = world.getEntities();

    std::vector<std::size_t> toRemove;
    toRemove.reserve(entities.size());

    for (auto &[playerId, playerPtr] : world.getPlayers())
    {
        if (!playerPtr) continue;

        float px      = playerPtr->getX();
        float py      = playerPtr->getY();
        float pRadius = playerPtr->getRadius();

        for (std::size_t i = 0; i < entities.size(); ++i)
        {
            const auto &entityPtr = entities[i];
            if (!entityPtr) 
            continue;

            float ex = entityPtr->getX();
            float ey = entityPtr->getY();
            float eRadius = entityPtr->getRadius();

            float dx    = px - ex;
            float dy    = py - ey;
            float distSq = dx * dx + dy * dy;
            float rSum   = pRadius + eRadius;

            if (distSq <= rSum * rSum)
            {
                if (auto food = dynamic_cast<Food*>(entityPtr.get()))
                {
                    /* Player eats Food */
                    playerPtr->addMass(food->getMass());
                    toRemove.push_back(i);
                }
                else if (auto boost = dynamic_cast<SpeedBoost*>(entityPtr.get()))
                {
                    /* Player picks up SpeedBoost */
                    float oldSpeed = playerPtr->getSpeed();
                    float newSpeed = oldSpeed * boost->getMultiplier();
                    playerPtr->setSpeed(newSpeed);

                    Logger::info("Player {} got SpeedBoost {} => speed: {} -> {}",
                                 playerPtr->getId(), boost->getId(),
                                 oldSpeed, newSpeed);

                    toRemove.push_back(i);
                }
            }
        }
    }

    std::sort(toRemove.begin(), toRemove.end());
    toRemove.erase(std::unique(toRemove.begin(), toRemove.end()), toRemove.end());

    for (auto it = toRemove.rbegin(); it != toRemove.rend(); ++it)
    {
        world.removeEntityAt(*it);
    }
}
