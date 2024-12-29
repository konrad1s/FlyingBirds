#include "Player.h"
#include "Logger.h"
#include <cmath>

Player::Player(uint32_t _id)
    : id(_id), position(0.f, 0.f), speed(100.f), size(100), angle(0.f)
{
}

uint32_t Player::getId() const
{
    return id;
}

Components::Position Player::getPosition() const
{
    return position;
}

void Player::setPosition(const Components::Position& pos)
{
    position = pos;
}

Components::Speed Player::getSpeed() const
{
    return speed;
}

void Player::setSpeed(const Components::Speed& vel)
{
    speed = vel;
}

float Player::getAngle() const
{
    return angle;
}

void Player::setAngle(const float a)
{
    angle = a;
}

Components::Size Player::getSize() const
{
    return size;
}

void Player::grow(int amount)
{
}

void Player::update(float deltaTime)
{
    float oldX = position.position.x;
    float oldY = position.position.y;

    position.position.x += std::cos(angle) * speed.speed * deltaTime;
    position.position.y += std::sin(angle) * speed.speed * deltaTime;

    Logger::info("Player {} moved from ({}, {}) to ({}, {})",
                  id, oldX, oldY, position.position.x, position.position.y);
}
