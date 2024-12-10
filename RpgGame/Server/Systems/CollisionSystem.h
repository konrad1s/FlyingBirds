
#pragma once

#include "World.h"

class CollisionSystem
{
public:
    void update(World &world, float deltaTime);

private:
    bool circlesCollide(const sf::Vector2f &posA, float radiusA, const sf::Vector2f &posB, float radiusB);
};
