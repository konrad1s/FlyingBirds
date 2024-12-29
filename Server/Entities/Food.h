#pragma once

#include "Position.h"

class Food
{
public:
    Food(const Components::Position& pos, float radius = 5.f);

    float getX() const;
    float getY() const;
    float getRadius() const;

private:
    Components::Position position;
    float radius;
};
