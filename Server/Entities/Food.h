#pragma once

#include "Entity.h"

class Food : public Entity
{
public:
    Food(uint32_t id, float x, float y, float massValue = 1000.f)
        : Entity(id, EntityType::Food, x, y, massValue)
    {
    }

    void update(float deltaTime, float xBoundary, float yBoundary) override {}
};
