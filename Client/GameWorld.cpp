#include "GameWorld.h"
#include "ResourceManager.h"
#include "Logger.h"
#include <unordered_set>
#include <algorithm>
#include <exception>
#include <cmath>

void GameWorld::update(float dt)
{
    for (auto &[id, playerPtr] : players)
    {
        playerPtr->update(dt);
    }

    for (auto &[id, entityPtr] : entities)
    {
        entityPtr->update(dt);
    }
}

void GameWorld::render(sf::RenderWindow &window)
{
    /* Draw all players */
    for (auto &[id, playerPtr] : players)
    {
        playerPtr->render(window);
    }

    /* Draw other entities */
    for (auto &[id, entityPtr] : entities)
    {
        entityPtr->render(window);
    }
}

void GameWorld::updateFromServer(const network::ServerToClient &message)
{
    /* Track which IDs are still valid in this update */
    std::unordered_set<uint32_t> updatedPlayerIds;
    std::unordered_set<uint32_t> updatedEntityIds;

    for (const auto &ent : message.entities())
    {
        uint32_t entId = ent.id();

        if (ent.entitytype() == network::ServerToClient::Entity::PLAYER)
        {
            updatedPlayerIds.insert(entId);

            /* Check if this player is already added */
            auto it = players.find(entId);
            if (it == players.end())
            {
                auto newPlayer = factory.createPlayer(
                    ent.position().x(),
                    ent.position().y(),
                    ent.has_mass() ? ent.mass() : 0.f
                );
                if (ent.has_speedboostactive())
                {
                    newPlayer->setSpeedBoost(ent.speedboostactive());
                }
                if (ent.has_protectionactive())
                {
                    newPlayer->setProtection(ent.protectionactive());
                }
                players.emplace(entId, std::move(newPlayer));
            }
            else
            {
                it->second->setPosition(ent.position().x(), ent.position().y());
                if (ent.has_mass())
                {
                    it->second->setMass(ent.mass());
                }
                if (ent.has_speedboostactive())
                {
                    it->second->setSpeedBoost(ent.speedboostactive());
                }
                if (ent.has_protectionactive())
                {
                    it->second->setProtection(ent.protectionactive());
                }
            }
        }
        else
        {
            updatedEntityIds.insert(entId);

            /* Check if this entity is already added */
            auto it = entities.find(entId);
            if (it == entities.end())
            {
                switch (ent.entitytype())
                {
                case network::ServerToClient::Entity::FOOD:
                {
                    auto newFood = factory.createFood(ent.position().x(), ent.position().y(), ent.mass());
                    entities.emplace(entId, std::move(newFood));
                    break;
                }
                case network::ServerToClient::Entity::SPEEDBOOST:
                {
                    auto newBoost = factory.createSpeedBoost(ent.position().x(), ent.position().y());
                    entities.emplace(entId, std::move(newBoost));
                    break;
                }
                case network::ServerToClient::Entity::PROTECTION:
                {
                    auto newBoost = factory.createProtection(ent.position().x(), ent.position().y());
                    entities.emplace(entId, std::move(newBoost));
                    break;
                }
                default:
                    Logger::warning("Unknown entity type from server");
                    break;
                }
            }
            else
            {
                it->second->setPosition(ent.position().x(), ent.position().y());

                if (auto f = dynamic_cast<Food*>(it->second.get()))
                {
                    f->setMass(ent.mass());
                }
            }
        }
    }

    /* Remove stale players and entities */
    for (auto it = players.begin(); it != players.end(); )
    {
        if (updatedPlayerIds.find(it->first) == updatedPlayerIds.end())
        {
            it = players.erase(it);
        }
        else
        {
            ++it;
        }
    }

    for (auto it = entities.begin(); it != entities.end(); )
    {
        if (updatedEntityIds.find(it->first) == updatedEntityIds.end())
        {
            it = entities.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
