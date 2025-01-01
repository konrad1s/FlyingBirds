#include "GameWorld.h"
#include "Logger.h"
#include "AngleUtils.h"
#include "ConfigServer.h"
#include "MovementSystem.h"
#include "CollisionSystem.h"
#include <random>
#include <chrono>
#include <memory>

GameWorld::GameWorld()
{
    rng.seed(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));

    systems.push_back(std::make_unique<MovementSystem>());
    systems.push_back(std::make_unique<CollisionSystem>());
}

void GameWorld::addPlayer(uint32_t id)
{
    auto player = std::make_unique<Player>(id);

    std::uniform_real_distribution<float> dist(0.f, ConfigServer::worldSize);
    player->setPosition(dist(rng), dist(rng));

    players[id] = std::move(player);
    Logger::info("Added player {} to the game at position ({}, {}).",
                 id,
                 players[id]->getX(),
                 players[id]->getY());
}

void GameWorld::removePlayer(uint32_t id)
{
    if (players.erase(id))
    {
        Logger::info("Removed player {} from the game.", id);
    }
    else
    {
        Logger::warning("Attempted to remove non-existent player {} from the game.", id);
    }
}

void GameWorld::spawnFood()
{
    std::uniform_int_distribution<int> foodCountDist(5, 10);
    std::uniform_real_distribution<float> positionDist(0.f, ConfigServer::worldSize);

    int totalNewFood = 0;
    for (auto &[playerId, playerPtr] : players)
    {
        int foodCount = foodCountDist(rng);
        totalNewFood += foodCount;

        for (int i = 0; i < foodCount; ++i)
        {
            static uint32_t foodId = 0;
            Food f(foodId++, positionDist(rng), positionDist(rng));
            foods.push_back(std::move(f));
        }
    }

    Logger::info("Spawned {} new food items. Total food count: {}", totalNewFood, foods.size());
}

void GameWorld::updatePlayerAngle(uint32_t id, float angleDegrees)
{
    auto it = players.find(id);

    if (it != players.end())
    {
        float angleRadians = AngleUtils::degreesToRadians(angleDegrees);
        it->second->setAngle(angleRadians);

        Logger::debug("Updated player {} angle to {} degrees ({} radians).",
                     id, angleDegrees, angleRadians);
    }
    else
    {
        Logger::warning("Attempted to update angle for non-existent player {}.", id);
    }
}

void GameWorld::update(float dt)
{
    for (auto &system : systems)
    {
        system->update(*this, dt);
    }
}

const std::unordered_map<uint32_t, std::unique_ptr<Player>> &GameWorld::getPlayers() const
{
    return players;
}

const std::vector<Food> &GameWorld::getFood() const
{
    return foods;
}

void GameWorld::removeFoodAt(std::size_t index)
{
    if (index < foods.size())
    {
        foods.erase(foods.begin() + index);
    }
}
