#include "GameWorld.h"
#include <chrono>
#include "ConfigServer.h"
#include "Logger.h"
#include "MovementSystem.h"
#include "CollisionSystem.h"
#include "EntitySpawnSystem.h"
#include <random>
#include <algorithm>
#include <memory>

GameWorld::GameWorld(EventBus &eventBus)
{
    rng.seed(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));

    systems.push_back(std::make_unique<MovementSystem>());
    systems.push_back(std::make_unique<CollisionSystem>(eventBus));
    systems.push_back(std::make_unique<EntitySpawnSystem>(*this));
}

void GameWorld::addPlayer(uint32_t id, const std::string &nickname)
{
    std::string uniqueNickname = nickname.substr(0, 15);
    int postfix = 1;

    auto nicknameExists = [&](const std::string &name) -> bool
    {
        for (const auto &pair : players)
        {
            if (pair.second->getNickname() == name)
                return true;
        }
        return false;
    };

    while (nicknameExists(uniqueNickname))
    {
        std::string newNickname = nickname + "_" + std::to_string(postfix);
        uniqueNickname = newNickname;

        ++postfix;
    }

    std::uniform_real_distribution<float> xPosDist(ConfigServer::xWorldMargin,
                                                   ConfigServer::xWorldSize - ConfigServer::xWorldMargin);
    std::uniform_real_distribution<float> yPosDist(ConfigServer::yWorldMargin,
                                                   ConfigServer::yWorldSize - ConfigServer::yWorldMargin);

    constexpr float minSpawnDistSq = 100.f * 100.f;
    constexpr int MAX_ATTEMPTS = 100;

    float spawnX = 0.0f;
    float spawnY = 0.0f;
    bool validPositionFound = false;

    /* TODO: Naive approach - to be changed later */
    for (int attempt = 0; attempt < MAX_ATTEMPTS; ++attempt)
    {
        float candidateX = xPosDist(rng);
        float candidateY = yPosDist(rng);

        bool tooClose = false;
        for (const auto &playerPair : players)
        {
            const auto &existingPlayer = playerPair.second;
            float distSq = distanceSquared(candidateX, candidateY,
                                           existingPlayer->getX(), existingPlayer->getY());
            if (distSq < minSpawnDistSq)
            {
                tooClose = true;
                break;
            }
        }

        if (!tooClose)
        {
            spawnX = candidateX;
            spawnY = candidateY;
            validPositionFound = true;
            break;
        }
    }

    if (!validPositionFound)
    {
        spawnX = xPosDist(rng);
        spawnY = yPosDist(rng);
        Logger::warning("Could not find a valid non-overlapping spawn location for player '{}'. Using random anyway.", uniqueNickname);
    }

    auto player = std::make_unique<Player>(id, spawnX, spawnY, 
                                           2500.f, 
                                           100.f, 
                                           uniqueNickname);

    players[id] = std::move(player);

    Logger::info("Added player {} ('{}') to the game at position ({}, {}).",
                 id, uniqueNickname, 
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

Player *GameWorld::findPlayerById(uint32_t playerId)
{
    auto it = players.find(playerId);
    if (it != players.end())
    {
        return it->second.get();
    }
    return nullptr;
}

void GameWorld::addEntity(std::unique_ptr<Entity> entity)
{
    entities.push_back(std::move(entity));
}

void GameWorld::removeEntityAt(std::size_t index)
{
    if (index < entities.size())
    {
        entities.erase(entities.begin() + index);
    }
}

void GameWorld::updatePlayerAngle(uint32_t playerId, float angleDegrees)
{
    if (Player *p = findPlayerById(playerId))
    {
        float radians = angleDegrees * 3.14159f / 180.f;
        p->setAngle(radians);
    }
}

void GameWorld::update(float deltaTime)
{
    for (auto &system : systems)
    {
        system->update(*this, deltaTime);
    }
}

float GameWorld::distanceSquared(float x1, float y1, float x2, float y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    return dx * dx + dy * dy;
}
