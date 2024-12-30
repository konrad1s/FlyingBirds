#include "Player.h"
#include <cmath>
#include <algorithm>

Player::Player()
    : mass(100.f)
{
}

void Player::update(float deltaTime)
{
}

void Player::render(sf::RenderWindow &window)
{
    float radius = mass.getRadius();
    float baseRadius = 10.f;
    float scaleFactor = radius / baseRadius;
    if (scaleFactor < 0.1f)
    {
        scaleFactor = 0.1f;
    }

    sprite.sprite.setPosition(transform.x, transform.y);
    sprite.sprite.setScale(scaleFactor, scaleFactor);

    window.draw(sprite.sprite);
}

void Player::setPosition(float x, float y)
{
    transform.x = x;
    transform.y = y;
}

float Player::getX() const
{
    return transform.x;
}

float Player::getY() const
{
    return transform.y;
}

float Player::getMass() const
{
    return mass.getValue();
}

void Player::setMass(float newMass)
{
    mass = Components::Mass(newMass);
}

void Player::setTexture(sf::Texture *texture)
{
    if (texture)
    {
        sprite.sprite.setTexture(*texture, true);
        sf::FloatRect bounds = sprite.sprite.getLocalBounds();
        sprite.sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    }
}
