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
    Components::Sprite sprite;
    Components::Transform transform;
};
