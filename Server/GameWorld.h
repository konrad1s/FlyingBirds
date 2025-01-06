#pragma once

#include <vector>
#include <memory>
#include <random>
#include <unordered_map>
#include "Entity.h"
#include "Player.h"
#include "Food.h"
#include "SpeedBoost.h"
#include "ISystem.h"

class GameWorld
{
public:
    GameWorld();

    void addPlayer(uint32_t id, const std::string &nickname);
    void removePlayer(uint32_t playerId);
    Player *findPlayerById(uint32_t playerId);

    void addEntity(std::unique_ptr<Entity> entity);
    void removeEntityAt(std::size_t index);

    const std::unordered_map<uint32_t, std::unique_ptr<Player>> &getPlayers() const { return players; }
    const std::vector<std::unique_ptr<Entity>> &getEntities() const { return entities; }

    void update(float deltaTime);
    void updatePlayerAngle(uint32_t playerId, float angleDegrees);

    std::mt19937 &getRNG() { return rng; }

private:
    std::unordered_map<uint32_t, std::unique_ptr<Player>> players;
    std::vector<std::unique_ptr<Entity>> entities;

    std::vector<std::unique_ptr<ISystem>> systems;

    std::mt19937 rng;
};