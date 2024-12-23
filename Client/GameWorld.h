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

    void updateFromServer(const network::ServerToClient &message);

private:
    std::unordered_map<uint32_t, std::unique_ptr<Player>> players;
    std::unordered_map<uint32_t, std::unique_ptr<Food>> foods;
};
