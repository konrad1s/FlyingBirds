#pragma once

#include <cmath>
#include <algorithm>
#include <vector>
#include <string>
#include "Entity.h"
#include "Speed.h"

class Player : public Entity
{
public:
    Player(uint32_t id, float x, float y, float initialMass, float speedVal, const std::string &nick);

    float getSpeed() const;
    void setSpeed(float s);

    float getAngle() const;
    void setAngle(float a);

    const std::string& getNickname() const;
    void setNickname(const std::string& nick);

    void addMass(float m);

    bool isShieldActive() const;
    bool isSpeedBoostActive() const;

    void update(float deltaTime) override;

    void applySpeedBoost(float multiplier, float duration);
    void applyProtection(float duration);

private:
    void updateSpeedBoosts(float deltaTime);
    void updateProtections(float deltaTime);

private:
    float angle;
    std::string nickname;
    Components::Speed speed;

    struct ActiveSpeedBoost
    {
        float multiplier;
        float remainingDuration;
        ActiveSpeedBoost(float m, float d)
            : multiplier(m), remainingDuration(d) {}
    };
    std::vector<ActiveSpeedBoost> activeSpeedBoosts;

    struct ActiveProtection
    {
        float remainingDuration;
        ActiveProtection(float d) : remainingDuration(d) {}
    };
    std::vector<ActiveProtection> activeProtections;
};
