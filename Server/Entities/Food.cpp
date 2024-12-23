#include "Food.h"

Food::Food(const Components::Position& pos, float _radius)
    : position(pos), radius(_radius)
{

}

const Components::Position& Food::getPosition() const
{
    return position;
}

float Food::getRadius() const
{
    return radius;
}
