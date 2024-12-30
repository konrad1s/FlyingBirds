
#pragma once

#include "ISystem.h"
#include "GameWorld.h"

class CollisionSystem : public ISystem
{
public:
    void update(GameWorld& world, float deltaTime) override;
};
