#include "Player.h"
#include <algorithm>
#include <cmath>
#include "Logger.h"
#include "ConfigServer.h"

Player::Player(uint32_t id, float x, float y, float initialMass, float speedVal, const std::string &nick)
    : Entity(id, EntityType::Player, x, y, initialMass),
      angle(0.f)
{
    speed.value = speedVal;
    setNickname(nick);
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

const std::string& Player::getNickname() const
{
    return nickname;
}

void Player::setNickname(const std::string& nick)
{
    nickname = nick;
}

void Player::addMass(float m)
{
    mass.addMass(m);
}

bool Player::isShieldActive() const
{
    return !activeProtections.empty();
}

bool Player::isSpeedBoostActive() const
{
    return !activeSpeedBoosts.empty();
}

void Player::update(float deltaTime)
{
    updateSpeedBoosts(deltaTime);
    updateProtections(deltaTime);

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

void Player::applyProtection(float duration)
{
    activeProtections.emplace_back(duration);
}

void Player::updateProtections(float deltaTime)
{
    for (auto it = activeProtections.begin(); it != activeProtections.end();)
    {
        it->remainingDuration -= deltaTime;
        if (it->remainingDuration <= 0.f)
        {
            it = activeProtections.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
