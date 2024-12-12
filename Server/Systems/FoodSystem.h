
#pragma once

#include "World.h"
#include <SFML/System/Vector2.hpp>

class FoodSystem
{
public:
    void spawnFood(World &world, int count, const sf::Vector2f &minPos, const sf::Vector2f &maxPos);
    void update(World &world, float deltaTime);

private:
    float spawnTimer = 0.f;
    const float spawnInterval = 5.f;

    void spawnFood(World &world, int count, const sf::Vector2f &minPos, const sf::Vector2f &maxPos);
};
