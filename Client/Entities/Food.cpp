#include "Food.h"

Food::Food()
    : size(10.0f)
{
}

void Food::update(float deltaTime)
{
}

void Food::render(sf::RenderWindow &window)
{
    if (sprite.sprite.getTexture())
    {
        sprite.sprite.setPosition(transform.x, transform.y);
        window.draw(sprite.sprite);
    }
}

void Food::setPosition(float x, float y)
{
    transform.x = x;
    transform.y = y;
}

void Food::setTexture(sf::Texture* texture)
{
    if (texture)
    {
        sprite.sprite.setTexture(*texture, true);
        sf::FloatRect bounds = sprite.sprite.getLocalBounds();
        sprite.sprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    }
}

float Food::getX() const
{
    return transform.x;
}

float Food::getY() const
{
    return transform.y;
}
