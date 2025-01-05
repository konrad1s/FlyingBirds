#include "Protection.h"

void Protection::update(float deltaTime)
{
}

void Protection::render(sf::RenderWindow& window)
{
    float diameter = 22.f;
    scaleSpriteToDiameter(diameter);

    sprite.sprite.setPosition(transform.x, transform.y);
    window.draw(sprite.sprite);
}

void Protection::setTexture(const std::shared_ptr<sf::Texture>& text)
{
    if (text)
    {
        sprite.sprite.setTexture(*text, true);
        texture = text;
    }
}
