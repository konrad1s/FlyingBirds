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

GameWorld::GameWorld()
{
    rng.seed(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));

    systems.push_back(std::make_unique<MovementSystem>());
    systems.push_back(std::make_unique<CollisionSystem>());
    systems.push_back(std::make_unique<EntitySpawnSystem>());
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

    std::uniform_real_distribution<float> xPos(ConfigServer::xWorldMargin,
                                               ConfigServer::xWorldSize - ConfigServer::xWorldMargin);
    std::uniform_real_distribution<float> yPos(ConfigServer::yWorldMargin,
                                               ConfigServer::yWorldSize - ConfigServer::yWorldMargin);

    auto player = std::make_unique<Player>(id, xPos(rng), yPos(rng), 2500.f, 100.f, uniqueNickname);

    players[id] = std::move(player);

    Logger::info("Added player {} ('{}') to the game at position ({}, {}).", 
                 id, uniqueNickname, players[id]->getX(), players[id]->getY());
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
