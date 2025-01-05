#pragma once

#include <SFML/Graphics.hpp>
#include "Transform.h"
#include "Sprite.h"

class Entity
{
public:
    virtual ~Entity() = default;
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow &window) = 0;

    float getX() const { return transform.x; }
    float getY() const { return transform.y; }

    void setPosition(float x, float y)
    {
        transform.x = x;
        transform.y = y;
    }

protected:
    void scaleSpriteToDiameter(float diameter)
    {
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
    }

protected:
    Components::Sprite sprite;
    Components::Transform transform;
};
