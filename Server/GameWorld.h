#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <chrono>
#include <random> 
#include "Player.h"
#include "Food.h"
#include "ISystem.h"

class GameWorld
{
public:
    GameWorld();

    void addPlayer(uint32_t id);
    void removePlayer(uint32_t id);

    void spawnFood();
    void updatePlayerAngle(uint32_t id, float angleDegrees);

    void update(float dt);

    const std::unordered_map<uint32_t, std::unique_ptr<Player>>& getPlayers() const;
    const std::vector<Food>& getFood() const;

    void removeFoodAt(std::size_t index);

private:
    std::unordered_map<uint32_t, std::unique_ptr<Player>> players;
    std::vector<Food> foods;

    std::vector<std::unique_ptr<ISystem>> systems;

    std::mt19937 rng;
};