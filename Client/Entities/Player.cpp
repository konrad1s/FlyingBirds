#include "Player.h"

Player::Player()
    : health(100, 100)
{
}

void Player::update(float deltaTime)
{
}

void Player::render(sf::RenderWindow &window)
{
    sprite.sprite.setPosition(transform.x, transform.y);
    window.draw(sprite.sprite);
}

void Player::setPosition(float x, float y)
{
    transform.x = x;
    transform.y = y;
}

void Player::setTexture(sf::Texture* texture)
{
    sprite.sprite.setTexture(*texture, true);
}
