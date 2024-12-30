#pragma once

#include "Position.h"
#include "Mass.h"

class Food
{
public:
    Food(float x, float y, float massValue = 10.f);

    float getX() const;
    float getY() const;

    float getMass() const;
    void  setMass(float m);

    float getRadius() const;

private:
    Components::Position position;
    Components::Mass     mass;
};