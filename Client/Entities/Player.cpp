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
    animation.update(deltaTime);
}

void Player::render(sf::RenderWindow &window)
{
    animation.applyToSprite(sprite.sprite);

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

Components::Animation &Player::getAnimation()
{
    return animation;
}

void Player::setTexture(const std::shared_ptr<sf::Texture> &texture)
{
    defaultTexture = texture;
}

const std::shared_ptr<sf::Texture> &Player::getTexture() const
{
    return defaultTexture;
}

void Player::setProtection(bool enabled)
{
    protectionEnabled = enabled;
}

void Player::setSpeedBoost(bool enabled)
{
    speedBoostEnabled = enabled;
}
