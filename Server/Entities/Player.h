#pragma once

#include <cmath>
#include <algorithm>
#include <vector>
#include "Entity.h"
#include "Speed.h"

class Player : public Entity
{
public:
    Player(uint32_t id, float x, float y, float initialMass, float speedVal);

    float getSpeed() const;
    void setSpeed(float s);

    float getAngle() const;
    void setAngle(float a);

    void addMass(float m);

    void update(float deltaTime) override;

    void applySpeedBoost(float multiplier, float duration);
    void updateSpeedBoosts(float deltaTime);

private:
    float angle;
    Components::Speed speed;

    struct ActiveSpeedBoost
    {
        float multiplier;
        float remainingDuration;

        ActiveSpeedBoost(float m, float d)
            : multiplier(m), remainingDuration(d) {}
    };

    std::vector<ActiveSpeedBoost> activeSpeedBoosts;
};
