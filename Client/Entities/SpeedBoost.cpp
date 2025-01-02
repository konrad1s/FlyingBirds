#include "SpeedBoost.h"

void SpeedBoost::update(float deltaTime)
{
}

void SpeedBoost::render(sf::RenderWindow &window)
{
    float diameter = 30.f;

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

void SpeedBoost::setTexture(const std::shared_ptr<sf::Texture> &texture_)
{
    if (texture_)
    {
        sprite.sprite.setTexture(*texture_, true);
        sf::FloatRect bounds = sprite.sprite.getLocalBounds();
        sprite.sprite.setOrigin(bounds.width * 0.5f, bounds.height * 0.5f);

        texture = texture_;
    }
}
