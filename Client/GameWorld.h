#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <unordered_map>
#include "network.pb.h"
#include "Player.h"
#include "Food.h"

class GameWorld
{
public:
    GameWorld() = default;

    void update(float dt);
    void render(sf::RenderWindow &window);

    void setMyPlayerId(uint32_t id) { myPlayerId = id; }
    uint32_t getMyPlayerId() { return myPlayerId; }

    void updateFromServer(const network::ServerToClient &message);

    const std::unordered_map<uint32_t, std::unique_ptr<Player>> &getPlayers() const { return players; }
    const std::unordered_map<uint32_t, std::unique_ptr<Food>> &getFoods() const { return foods; }

private:
    uint32_t myPlayerId;
    std::unordered_map<uint32_t, std::unique_ptr<Player>> players;
    std::unordered_map<uint32_t, std::unique_ptr<Food>> foods;
};
