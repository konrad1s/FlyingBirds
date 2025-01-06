#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <unordered_map>
#include "network.pb.h"
#include "Player.h"
#include "EntityFactory.h"
#include "Entity.h"

class GameWorld
{
public:
    GameWorld() = default;

    void update(float dt);
    void render(sf::RenderWindow &window);

    void setMyPlayerId(uint32_t id) { myPlayerId = id; }
    uint32_t getMyPlayerId() const { return myPlayerId; }

    void updateFromServer(const network::ServerToClient &message);

    const std::unordered_map<uint32_t, std::unique_ptr<Player>>& getPlayers()  const { return players; }
    const std::unordered_map<uint32_t, std::unique_ptr<Entity>>& getEntities() const { return entities; }

private:
    void updateOrCreatePlayer(const network::ServerToClient::Entity &ent);
    void updateOrCreateEntity(const network::ServerToClient::Entity &ent);

private:
    uint32_t myPlayerId{0};

    EntityFactory factory;

    std::unordered_map<uint32_t, std::unique_ptr<Player>> players;
    std::unordered_map<uint32_t, std::unique_ptr<Entity>> entities;
};
