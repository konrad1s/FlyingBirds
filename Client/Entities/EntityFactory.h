#pragma once

#include <memory>
#include "Player.h"
#include "Food.h"
#include "SpeedBoost.h"

class EntityFactory
{
public:
    EntityFactory() = default;
    ~EntityFactory() = default;

    std::unique_ptr<Player> createPlayer(float x, float y, float mass);
    std::unique_ptr<Food> createFood(float x, float y, float mass);
    std::unique_ptr<SpeedBoost> createSpeedBoost(float x, float y);

private:
    int playerAssetId{0};
};
