#include "GameWorld.h"
#include "Logger.h"
#include <random>

GameWorld::GameWorld()
{
    rng.seed(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
}

void GameWorld::addPlayer(uint32_t id)
{
    auto player = std::make_unique<Player>(id);

    std::uniform_real_distribution<float> dist(0.f, 800.f);
    player->setPosition(Components::Position(dist(rng), dist(rng)));

    players[id] = std::move(player);
    Logger::info("Added player {} to the game at position ({}, {}).",
                 id,
                 players[id]->getPosition().position.x,
                 players[id]->getPosition().position.y);
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
    std::uniform_int_distribution<int> foodCountDist(0, 3);
    std::uniform_real_distribution<float> positionDist(0.f, 800.f);

    for (auto &[playerId, playerPtr] : players)
    {
        int foodCount = foodCountDist(rng);

        for (int i = 0; i < foodCount; ++i)
        {
            Food f(Components::Position(positionDist(rng), positionDist(rng)));
            foods.push_back(f);
        }
    }
    Logger::info("Spawned food. Total food count: {}", foods.size());
}

std::unordered_map<uint32_t, std::unique_ptr<Player>> &GameWorld::getPlayers()
{
    return players;
}

std::vector<Food> &GameWorld::getFood()
{
    return foods;
}
