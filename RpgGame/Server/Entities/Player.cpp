#include "Player.h"

Player::Player(uint32_t _id)
    : id(_id), position(0.f, 0.f), velocity(0.f), size(100)
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

void setPosition(const Components::Position& pos)
{
}

Components::Velocity Player::getVelocity() const
{
    return velocity;
}

void Player::setVelocity(const Components::Velocity& vel)
{
    velocity = vel;
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
}
