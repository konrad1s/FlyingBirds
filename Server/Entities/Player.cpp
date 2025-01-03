#include "Player.h"
#include <algorithm>
#include <cmath>
#include "Logger.h"
#include "ConfigServer.h"

Player::Player(uint32_t id, float x, float y, float initialMass, float speedVal)
    : Entity(id, EntityType::Player, x, y, initialMass),
      angle(0.f)
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

void Player::update(float deltaTime)
{
    updateSpeedBoosts(deltaTime);

    float dx = std::cos(angle) * speed.value * deltaTime;
    float dy = std::sin(angle) * speed.value * deltaTime;

    float newX = position.coords.x + dx;
    float newY = position.coords.y + dy;

    newX = std::clamp(newX, ConfigServer::xWorldMargin, ConfigServer::xWorldSize - ConfigServer::xWorldMargin);
    newY = std::clamp(newY, ConfigServer::yWorldMargin, ConfigServer::yWorldSize - ConfigServer::yWorldMargin);

    position.coords.x = newX;
    position.coords.y = newY;
}

void Player::applySpeedBoost(float multiplier, float duration)
{
    speed.value *= multiplier;

    activeSpeedBoosts.emplace_back(multiplier, duration);
}

void Player::updateSpeedBoosts(float deltaTime)
{
    for (auto it = activeSpeedBoosts.begin(); it != activeSpeedBoosts.end();)
    {
        it->remainingDuration -= deltaTime;
        if (it->remainingDuration <= 0.f)
        {
            speed.value /= it->multiplier;
            it = activeSpeedBoosts.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
