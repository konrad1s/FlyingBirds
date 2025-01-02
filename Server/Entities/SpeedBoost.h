#pragma once

#include "Entity.h"

class SpeedBoost : public Entity
{
public:
    SpeedBoost(uint32_t id, float x, float y,
               float speedMultiplier = 1.25f, float activeDuration = 4.f)
        : Entity(id, EntityType::SpeedBoost, x, y, 0.f),
          multiplier(speedMultiplier),
          duration(activeDuration)
    {
    }

    float getMultiplier() const
    {
        return multiplier;
    }

    float getDuration() const
    {
        return duration;
    }

    void update(float deltaTime, float xBoundary, float yBoundary) override
    {
    }

private:
    float multiplier;
    float duration;
};
