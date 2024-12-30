#include "Food.h"
#include <algorithm>

Food::Food()
    : mass(10.f)
{
}

void Food::update(float deltaTime)
{
}

void Food::render(sf::RenderWindow &window)
{
    float radius = mass.getRadius();
    float baseRadius = 5.f;
    float scaleFactor = radius / baseRadius;
    if (scaleFactor < 0.1f)
    {
        scaleFactor = 0.1f;
    }

    sprite.sprite.setPosition(transform.x, transform.y);
    sprite.sprite.setScale(scaleFactor, scaleFactor);

    window.draw(sprite.sprite);
}

void Food::setPosition(float x, float y)
{
    transform.x = x;
    transform.y = y;
}

float Food::getX() const
{
    return transform.x;
}

float Food::getY() const
{
    return transform.y;
}

float Food::getMass() const
{
    return mass.getValue();
}

void Food::setMass(float newMass)
{
    mass = Components::Mass(newMass);
}

void Food::setTexture(sf::Texture *texture)
{
    if (texture)
    {
        sprite.sprite.setTexture(*texture, true);
        sf::FloatRect bounds = sprite.sprite.getLocalBounds();
        sprite.sprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    }
}
