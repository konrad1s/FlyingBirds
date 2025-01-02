#include "Player.h"

Player::Player(uint32_t id, float x = 0.f, float y = 0.f, float initialMass = 5000.f, float speedVal = 100.f)
    : Entity(id, EntityType::Player, x, y, initialMass), angle(0.f)
{
    speed.value = speedVal;
}

float Player::getSpeed() const
{
    return speed.value;
}

void Player::setSpeed(float s)
{
    speed.value = s;
}

float Player::getAngle() const
{
    return angle;
}

void Player::setAngle(float a)
{
    angle = a;
}

void Player::addMass(float m)
{
    mass.addMass(m);
}

void Player::update(float deltaTime, float xBoundary, float yBoundary)
{
    float dx = std::cos(angle) * speed.value * deltaTime;
    float dy = std::sin(angle) * speed.value * deltaTime;

    float newX = position.coords.x + dx;
    float newY = position.coords.y + dy;

    newX = std::clamp(newX, 0.f, xBoundary);
    newY = std::clamp(newY, 0.f, yBoundary);

    position.coords.x = newX;
    position.coords.y = newY;
}
