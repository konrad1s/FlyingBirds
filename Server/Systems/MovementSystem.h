
#pragma once

#include "ISystem.h"
#include "GameWorld.h"

class MovementSystem : public ISystem
{
public:
    void update(GameWorld& world, float deltaTime) override;
};
