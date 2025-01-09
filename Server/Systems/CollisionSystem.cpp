#include "CollisionSystem.h"
#include "GameWorld.h"
#include "Player.h"
#include "Food.h"
#include "SpeedBoost.h"
#include "Protection.h"
#include "Logger.h"
#include "ConfigServer.h"
#include "Events.h"
#include <cmath>
#include <vector>
#include <algorithm>
#include <memory>
#include <unordered_set>

void CollisionSystem::update(GameWorld &world, float deltaTime)
{
    auto &entities = world.getEntities();

    std::vector<std::size_t> toRemove;
    toRemove.reserve(entities.size());

    std::unordered_set<uint32_t> attackingPlayers;

    /* 1. Handle collisions of players with entities */
    for (auto &[playerId, playerPtr] : world.getPlayers())
    {
        if (!playerPtr || playerPtr->getMass() <= 0.f)
            continue;

        const float px = playerPtr->getX();
        const float py = playerPtr->getY();
        const float pRadius = playerPtr->getRadius();

        for (std::size_t i = 0; i < entities.size(); ++i)
        {
            auto &entityPtr = entities[i];
            if (!entityPtr)
                continue;

            const float ex = entityPtr->getX();
            const float ey = entityPtr->getY();
            const float eRadius = entityPtr->getRadius();

            const float dx = px - ex;
            const float dy = py - ey;
            const float distSq = dx * dx + dy * dy;
            const float rSum = pRadius + eRadius;

            if (distSq <= rSum * rSum)
            {
                /* --- Food Collision --- */
                if (auto food = dynamic_cast<Food *>(entityPtr.get()))
                {
                    playerPtr->addMass(food->getMass());
                    toRemove.push_back(i);
                }
                /* --- SpeedBoost Collision --- */
                else if (auto boost = dynamic_cast<SpeedBoost *>(entityPtr.get()))
                {
                    playerPtr->applySpeedBoost(boost->getMultiplier(), boost->getDuration());
                    toRemove.push_back(i);
                }
                /* --- Protection Collision --- */
                else if (auto protection = dynamic_cast<Protection *>(entityPtr.get()))
                {
                    playerPtr->applyProtection(protection->getDuration());
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

    /* 2. Handle Player-vs-Player collisions */
    auto &players = world.getPlayers();

    for (auto it1 = players.begin(); it1 != players.end(); ++it1)
    {
        Player *player1 = (it1->second) ? it1->second.get() : nullptr;
        if (!player1 || player1->getMass() <= 0.f)
            continue;

        for (auto it2 = std::next(it1); it2 != players.end(); ++it2)
        {
            Player *player2 = (it2->second) ? it2->second.get() : nullptr;
            if (!player2 || player2->getMass() <= 0.f)
                continue;

            const float dx = player1->getX() - player2->getX();
            const float dy = player1->getY() - player2->getY();
            const float distSq = dx * dx + dy * dy;
            const float rSum = player1->getRadius() + player2->getRadius();

            if (distSq <= rSum * rSum)
            {
                if (player1->getMass() == player2->getMass())
                {
                    continue;
                }

                Player *bigger = (player1->getMass() > player2->getMass()) ? player1 : player2;
                Player *smaller = (bigger == player1) ? player2 : player1;

                if (smaller->isShieldActive())
                    continue;

                attackingPlayers.insert(bigger->getId());

                const float attackAmount = ConfigServer::playerCollisionAttackFactor * bigger->getMass();
                if (attackAmount <= 0.f)
                    continue;

                const float actualAmount = std::min(smaller->getMass(), attackAmount);

                bigger->addMass(actualAmount);
                smaller->setMass(smaller->getMass() - actualAmount);

                if (smaller->getMass() <= 0.f)
                {
                    smaller->setMass(0.f);
                    Events::PlayerDiedEvent event{ smaller->getId() };
                    eventBus.publish<Events::PlayerDiedEvent>(event);
                    Logger::info("Player {} dead.", smaller->getId());
                }
            }
        }
    }

    /* 3. Update Attacking States */
    for (auto &[playerId, playerPtr] : players)
    {
        if (playerPtr && playerPtr->getMass() > 0.f)
        {
            bool isAttacking = attackingPlayers.find(playerId) != attackingPlayers.end();
            playerPtr->setAttacking(isAttacking);
        }
    }
}
