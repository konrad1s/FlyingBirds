#pragma once

#include "ISystem.h"
#include "GameWorld.h"

class FoodSystem : public ISystem
{
public:
    explicit FoodSystem(float spawnInterval = 5.0f);

    void update(GameWorld &world, float deltaTime) override;

private:
    void spawnFood(GameWorld &world);

    float spawnInterval;
    float timeSinceLastSpawn;

    std::mt19937 rng;
    std::uniform_int_distribution<int> foodCountDist{1, 3};
    std::uniform_real_distribution<float> positionDist;
};