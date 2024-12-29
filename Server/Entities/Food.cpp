#include "Food.h"

Food::Food(const Components::Position& pos, float _radius)
    : position(pos), radius(_radius)
{
}

float Food::getX() const
{
    return position.coords.x;
}

float Food::getY() const
{
    return position.coords.y;
}

float Food::getRadius() const
{
    return radius;
}