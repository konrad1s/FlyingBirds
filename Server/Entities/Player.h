#pragma once

#include <cmath>
#include <algorithm>
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

    void update(float deltaTime, float xBoundary, float yBoundary) override;

private:
    float angle;
    Components::Speed speed;
};
