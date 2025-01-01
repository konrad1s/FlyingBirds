#include "Food.h"

Food::Food(uint32_t id, float x, float y, float massValue)
    : id(id), position(x, y), mass(massValue)
{
}

uint32_t Food::getId() const
{
    return id;
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