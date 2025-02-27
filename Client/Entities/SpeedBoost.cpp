#include "SpeedBoost.h"

void SpeedBoost::update(float deltaTime)
{
}

void SpeedBoost::render(sf::RenderWindow& window)
{
    float diameter = 22.f;
    scaleSpriteToDiameter(diameter);

    sprite.sprite.setPosition(transform.x, transform.y);
    window.draw(sprite.sprite);
}

void SpeedBoost::setTexture(const std::shared_ptr<sf::Texture>& text)
{
    if (text)
    {
        sprite.sprite.setTexture(*text, true);
        texture = text;
    }
}
