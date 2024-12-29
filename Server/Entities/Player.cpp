#include "Player.h"
#include "Logger.h"
#include <cmath>

Player::Player(uint32_t _id)
    : id(_id), position(0.f, 0.f), speed(100.f), size(100.f), angle(0.f)
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

float Player::getSpeedValue() const
{
    return speed.value;
}

void Player::setSpeedValue(float val)
{
    speed.value = val;
}

float Player::getSizeValue() const
{
    return size.value;
}

void Player::grow(float amount)
{
    size.value += amount;
}

void Player::update(float deltaTime)
{
    float dx = std::cos(angle) * speed.value * deltaTime;
    float dy = std::sin(angle) * speed.value * deltaTime;

    Logger::info("Player {} moved from ({}, {}) to ({}, {})",
                 id, position.coords.x, position.coords.y,
                 position.coords.x + dx, position.coords.y + dy);

    position.coords.x += dx;
    position.coords.y += dy;
}
