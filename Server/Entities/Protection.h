#pragma once

#include "Entity.h"

class Protection : public Entity
{
public:
    Protection(uint32_t id, float x, float y, float duration = 5.f)
        : Entity(id, EntityType::Protection, x, y, 0.f),
          duration(duration)
    {
    }

    float getDuration() const { return duration; }

    void update(float deltaTime) override
    {

    }

private:
    float duration;
};
