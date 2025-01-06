#include "Player.h"
#include "ResourceManager.h"
#include <cmath>
#include <algorithm>

Player::Player()
    : mass(100.f)
{
}

void Player::update(float deltaTime)
{
    if (attacking)
    {
        shootingAnimation.update(deltaTime);
    }
    else
    {
        flyingAnimation.update(deltaTime);
    }
}

void Player::render(sf::RenderWindow &window)
{
    if (attacking)
    {
        shootingAnimation.applyToSprite(sprite.sprite);
    }
    else
    {
        flyingAnimation.applyToSprite(sprite.sprite);
    }

    float radius = mass.getRadius();
    float diameter = 2.f * radius;

    scaleSpriteToDiameter(diameter);

    if (transform.x < lastX)
    {
        sprite.sprite.setScale(-std::abs(sprite.sprite.getScale().x), sprite.sprite.getScale().y);
    }
    else if (transform.x > lastX)
    {
        sprite.sprite.setScale(std::abs(sprite.sprite.getScale().x), sprite.sprite.getScale().y);
    }

    sprite.sprite.setPosition(transform.x, transform.y);

    if (protectionEnabled) 
    {
        sf::Sprite glowSprite = sprite.sprite;
        glowSprite.setColor(sf::Color(255, 255, 0));
        glowSprite.setScale(glowSprite.getScale() * 1.3f);
        window.draw(glowSprite, sf::BlendAdd);
    }

    window.draw(sprite.sprite);

    lastX = transform.x;
}

float Player::getMass() const
{
    return mass.getValue();
}

void Player::setMass(float newMass)
{
    mass = Components::Mass(newMass);
}

const std::shared_ptr<sf::Texture> &Player::getTexture() const
{
    auto &rm = ResourceManager::getInstance();

    std::string directory;

    if (mass.getValue() > 0.0f)
    {
        directory = "flying";
    }
    else
    {
        directory = "gameOver";
    }

    std::string key = "p" + std::to_string(assetId) + "_" + directory + "_" + std::to_string(1);
    std::string path = "players/player" + std::to_string(assetId + 1) + "/" + directory + "/a" + std::to_string(1) + ".png";
    
    return rm.acquire<sf::Texture>(key, path);
}

void Player::setProtection(bool enabled)
{
    protectionEnabled = enabled;
}

void Player::setSpeedBoost(bool enabled)
{
    speedBoostEnabled = enabled;
}
