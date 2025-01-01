#pragma once

#include "Position.h"
#include "Mass.h"

class Food
{
public:
    Food(uint32_t id, float x, float y, float massValue = 1000.f);

    uint32_t getId() const;

    float getX() const;
    float getY() const;

    float getMass() const;
    void  setMass(float m);

    float getRadius() const;

private:
    uint32_t id;
    Components::Position position;
    Components::Mass     mass;
};