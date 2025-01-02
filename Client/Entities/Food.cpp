#include "Food.h"
#include <algorithm>

Food::Food()
    : mass(10.f)
{
}

void Food::update(float deltaTime)
{
    animation.update(deltaTime);
}

void Food::render(sf::RenderWindow &window)
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

    sprite.sprite.setPosition(transform.x, transform.y);
    window.draw(sprite.sprite);
}

float Food::getMass() const
{
    return mass.getValue();
}

void Food::setMass(float newMass)
{
    mass = Components::Mass(newMass);
}

Components::Animation &Food::getAnimation()
{
    return animation;
}

void Food::setTexture(const std::shared_ptr<sf::Texture> &texture)
{
    if (texture)
    {
        sprite.sprite.setTexture(*texture, true);
        sf::FloatRect bounds = sprite.sprite.getLocalBounds();
        sprite.sprite.setOrigin(bounds.width * 0.5f, bounds.height * 0.5f);
    }
}
