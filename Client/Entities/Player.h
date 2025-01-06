#pragma once

#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "Transform.h"
#include "Sprite.h"
#include "Mass.h"
#include "Animation.h"
#include <string>

class Player : public Entity
{
public:
    Player();

    void update(float deltaTime) override;
    void render(sf::RenderWindow &window) override;

    float getMass() const;
    void setMass(float newMass);

    void setNickname(const std::string& nick) { nickname = nick; }
    const std::string& getNickname() const { return nickname; }

    Components::Animation& getFlyingAnimation() { return flyingAnimation; }
    Components::Animation& getShootingAnimation() { return shootingAnimation; }

    void setTexture(const std::shared_ptr<sf::Texture> &texture);
    const std::shared_ptr<sf::Texture> &getTexture() const;

    void setProtection(bool enabled);
    bool isProtectionActive() const { return protectionEnabled; }
    void setSpeedBoost(bool enabled);
    bool isSpeedBoostActive() const { return speedBoostEnabled; }
    void setAttacking(bool attack) { attacking = attack; }
    bool isAttacking() const { return attacking; }

private:
    Components::Animation flyingAnimation;
    Components::Animation shootingAnimation;
    std::shared_ptr<sf::Texture> defaultTexture;

    Components::Mass mass;
    bool attacking{false};
    bool protectionEnabled{false};
    bool speedBoostEnabled{false};
    float speed{100.f};
    float lastX{0.f};

    std::string nickname; 
};
