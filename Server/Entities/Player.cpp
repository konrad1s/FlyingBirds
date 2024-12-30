#include "Player.h"

Player::Player(uint32_t _id)
    : id(_id), angle(0.f), position(0.f, 0.f), speed(100.f), mass(100.f)
{
}

uint32_t Player::getId() const
{
    return id;
}

float Player::getAngle() const
{
    return angle;
}

void Player::setAngle(float a)
{
    angle = a;
}

float Player::getX() const
{
    return position.coords.x;
}

float Player::getY() const
{
    return position.coords.y;
}

void Player::setPosition(float x, float y)
{
    position.coords.x = x;
    position.coords.y = y;
}

float Player::getSpeed() const
{
    return speed.value;
}

void Player::setSpeed(float s)
{
    speed.value = s;
}

float Player::getMass() const
{
    return mass.getValue();
}

void Player::setMass(float m)
{
    mass.setValue(m);
}

void Player::addMass(float m)
{
    mass.addMass(m);
}

float Player::getRadius() const
{
    return mass.getRadius();
}

void Player::update(float deltaTime, float xBoundary, float yBoundary)
{
    float dx = std::cos(angle) * speed.value * deltaTime;
    float dy = std::sin(angle) * speed.value * deltaTime;

    float newX = position.coords.x + dx;
    float newY = position.coords.y + dy;

    newX = std::clamp(newX, 0.0f, xBoundary);
    newY = std::clamp(newY, 0.0f, yBoundary);

    position.coords.x = newX;
    position.coords.y = newY;
}