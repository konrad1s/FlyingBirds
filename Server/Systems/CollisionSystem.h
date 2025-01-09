
#pragma once

#include "ISystem.h"
#include "GameWorld.h"
#include "EventBus.h"

class CollisionSystem : public ISystem
{
public:
    CollisionSystem(EventBus &bus)
        : eventBus(bus)
    {
    }
    void update(GameWorld &world, float deltaTime) override;

private:
    EventBus &eventBus;
};
