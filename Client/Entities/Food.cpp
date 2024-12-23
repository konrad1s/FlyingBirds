#include "Food.h"

Food::Food()
{
}

void Food::update(float deltaTime)
{
}

void Food::render(sf::RenderWindow &window)
{
    sprite.sprite.setPosition(transform.x, transform.y);
    window.draw(sprite.sprite);
}

void Food::setPosition(float x, float y)
{
    transform.x = x;
    transform.y = y;
}

void Food::setTexture(sf::Texture* texture)
{
    sprite.sprite.setTexture(*texture, true);
}
