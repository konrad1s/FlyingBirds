#include "Player.h"
#include "ResourceManager.h"

Player::Player()
{
    auto &resourceManager = ResourceManager::getInstance();
    resourceManager.load<sf::Texture>("player", "../../../assets/textures/player.png");

    transform.x = 100.0f;
    transform.y = 100.0f;

    sprite.sprite.setTexture(*resourceManager.get<sf::Texture>("player"));
}

void Player::update(float deltaTime)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        transform.x -= speed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        transform.x += speed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        transform.y -= speed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        transform.y += speed * deltaTime;
    }

    sprite.sprite.setPosition(transform.x, transform.y);
}

void Player::render(sf::RenderWindow &window)
{
    window.draw(sprite.sprite);
}
