#include "GameWorld.h"
#include "Logger.h"
#include <random>
#include <chrono>

GameWorld::GameWorld()
{

}

void GameWorld::addPlayer(uint32_t id)
{
    auto player = std::make_unique<Player>(id);

    static std::mt19937 rng(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_real_distribution<float> dist(0.f, 2000.f);
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

std::unordered_map<uint32_t, std::unique_ptr<Player>> &GameWorld::getPlayers()
{
    return players;
}

std::vector<Food> &GameWorld::getFood()
{
    return foods;
}
