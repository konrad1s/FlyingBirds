#pragma once

#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "Sprite.h"
#include "Transform.h"

class Food : public Entity
{
public:
    Food();
    void update(float deltaTime) override;
    void render(sf::RenderWindow &window) override;

    void setPosition(float x, float y);
    void setTexture(sf::Texture* texture);
    float getX() const;
    float getY() const;

    float getSize() const { return size; }
    void setSize(float newSize) { size = newSize; }

private:

    Components::Sprite sprite;
    Components::Transform transform;
    float size;
};
