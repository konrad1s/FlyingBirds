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

    sf::FloatRect localBounds = sprite.sprite.getLocalBounds();

    sprite.sprite.setOrigin(localBounds.width * 0.5f, localBounds.height * 0.5f);

    if (localBounds.width > 0.f && localBounds.height > 0.f)
    {
        float scaleX = diameter / localBounds.width;
        float scaleY = diameter / localBounds.height;
        sprite.sprite.setScale(scaleX, scaleY);
    }
    else
    {
        sprite.sprite.setScale(1.f, 1.f);
    }

    if (transform.x < lastX)
    {
        sprite.sprite.setScale(-std::abs(sprite.sprite.getScale().x), sprite.sprite.getScale().y);
    }
    else if (transform.x > lastX)
    {
        sprite.sprite.setScale(std::abs(sprite.sprite.getScale().x), sprite.sprite.getScale().y);
    }

    sprite.sprite.setPosition(transform.x, transform.y);
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
    if (texture)
    {
        sprite.sprite.setTexture(*texture, true);
        sf::FloatRect bounds = sprite.sprite.getLocalBounds();
        sprite.sprite.setOrigin(bounds.width * 0.5f, bounds.height * 0.5f);
    }
}
