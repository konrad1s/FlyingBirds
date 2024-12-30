#pragma once

#include <cstdint>
#include <cmath>
#include <algorithm>
#include "Position.h"
#include "Speed.h"
#include "Mass.h"

class Player
{
public:
    explicit Player(uint32_t id);

    uint32_t getId() const;

    float getAngle() const;
    void setAngle(float angleDegrees);

    float getX() const;
    float getY() const;
    void setPosition(float x, float y);

    float getSpeed() const;
    void setSpeed(float s);

    float getMass() const;
    void setMass(float m);
    void addMass(float m);

    float getRadius() const;

    void update(float deltaTime, float xBoundary, float yBoundary);

private:
    uint32_t id;
    float angle;

    Components::Position position;
    Components::Speed speed;
    Components::Mass mass;
};