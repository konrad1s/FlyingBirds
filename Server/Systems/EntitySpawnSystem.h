#pragma once

#include "ISystem.h"

class EntitySpawnSystem : public ISystem
{
public:
    void update(GameWorld &world, float deltaTime) override;

private:
    float timeSinceLastSpawn = 0.f;
    const float spawnInterval = 5.f;
};
