#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include "Player.h"
#include "Food.h"

class GameWorld
{
public:
    GameWorld();

    void addPlayer(uint32_t id);
    void removePlayer(uint32_t id);

    std::unordered_map<uint32_t, std::unique_ptr<Player>>& getPlayers();
    std::vector<Food>& getFood();

private:
    std::unordered_map<uint32_t, std::unique_ptr<Player>> players;
    std::vector<Food> foods;
};