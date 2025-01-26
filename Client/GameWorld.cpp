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
    std::vector<Player*> sortedPlayers;
    sortedPlayers.reserve(players.size());
    for (const auto &[id, playerPtr] : players)
    {
        sortedPlayers.emplace_back(playerPtr.get());
    }

    /* Sort players by mass */
    std::sort(sortedPlayers.begin(), sortedPlayers.end(),
              [](const Player* a, const Player* b) -> bool
              {
                  return a->getMass() > b->getMass();
              });

    /* Draw all players in sorted order */
    for (auto *playerPtr : sortedPlayers)
    {
        playerPtr->render(window);
    }

    /* Draw other entities */
    for (const auto &[id, entityPtr] : entities)
    {
        entityPtr->render(window);
    }
}


void GameWorld::updateFromServer(const network::ServerToClient &message)
{
    std::unordered_set<uint32_t> updatedPlayerIds;
    std::unordered_set<uint32_t> updatedEntityIds;

    for (const auto &ent : message.entities())
    {
        switch (ent.entitytype())
        {
        case network::ServerToClient::Entity::PLAYER:
            updatedPlayerIds.insert(ent.id());
            updateOrCreatePlayer(ent);
            break;

        case network::ServerToClient::Entity::FOOD:
        case network::ServerToClient::Entity::SPEEDBOOST:
        case network::ServerToClient::Entity::PROTECTION:
        default:
            updatedEntityIds.insert(ent.id());
            updateOrCreateEntity(ent);
            break;
        }
    }

    /* Do not erase Players for now
    std::erase_if(players, [&](auto &kv)
                  { return !updatedPlayerIds.contains(kv.first); });
    */

    std::erase_if(entities, [&](auto &kv)
                  { return !updatedEntityIds.contains(kv.first); });
}

void GameWorld::updateOrCreatePlayer(const network::ServerToClient::Entity &ent)
{
    const uint32_t entId = ent.id();
    auto it = players.find(entId);

    if (it == players.end())
    {
        auto newPlayer = factory.createPlayer(
            ent.position().x(),
            ent.position().y(),
            ent.has_mass() ? ent.mass() : 0.f);

        if (ent.has_speedboostactive())
            newPlayer->setSpeedBoost(ent.speedboostactive());
        if (ent.has_protectionactive())
            newPlayer->setProtection(ent.protectionactive());
        if (ent.has_attacking()) 
            newPlayer->setAttacking(ent.attacking());

        newPlayer->setNickname(ent.nickname());

        players.emplace(entId, std::move(newPlayer));
    }
    else
    {
        it->second->setPosition(ent.position().x(), ent.position().y());

        if (ent.has_mass())
            it->second->setMass(ent.mass());
        if (ent.has_speedboostactive())
            it->second->setSpeedBoost(ent.speedboostactive());
        if (ent.has_protectionactive())
            it->second->setProtection(ent.protectionactive());
        if (ent.has_attacking())
            it->second->setAttacking(ent.attacking());

        it->second->setNickname(ent.nickname());
    }
}

void GameWorld::updateOrCreateEntity(const network::ServerToClient::Entity &ent)
{
    const uint32_t entId = ent.id();
    auto it = entities.find(entId);

    if (it == entities.end())
    {
        switch (ent.entitytype())
        {
        case network::ServerToClient::Entity::FOOD:
        {
            auto newFood = factory.createFood(
                ent.position().x(),
                ent.position().y(),
                ent.mass());
            entities.emplace(entId, std::move(newFood));
            break;
        }
        case network::ServerToClient::Entity::SPEEDBOOST:
        {
            auto newBoost = factory.createSpeedBoost(
                ent.position().x(),
                ent.position().y());
            entities.emplace(entId, std::move(newBoost));
            break;
        }
        case network::ServerToClient::Entity::PROTECTION:
        {
            auto newProtection = factory.createProtection(
                ent.position().x(),
                ent.position().y());
            entities.emplace(entId, std::move(newProtection));
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

        if (auto *f = dynamic_cast<Food *>(it->second.get()))
        {
            f->setMass(ent.mass());
        }
    }
}
