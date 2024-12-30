#include "Food.h"

Food::Food(float x, float y, float massValue)
    : position(x, y), mass(massValue)
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

float Food::getMass() const
{
    return mass.getValue();
}

void Food::setMass(float m)
{
    mass.setValue(m);
}

float Food::getRadius() const
{
    return mass.getRadius();
}