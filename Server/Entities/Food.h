#pragma once

#include <SFML/System/Vector2.hpp>
#include "Position.h"

class Food
{
public:
    Food(const Components::Position& pos, float _radius = 5.f);

    const Components::Position& getPosition() const;
    float getRadius() const;

private:
    Components::Position position;
    float radius;
};
