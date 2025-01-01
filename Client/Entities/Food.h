#pragma once

#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "Transform.h"
#include "Sprite.h"
#include "Mass.h"

class Food : public Entity
{
public:
    Food();

    void update(float deltaTime) override;
    void render(sf::RenderWindow &window) override;

    void setPosition(float x, float y);
    float getX() const;
    float getY() const;

    float getMass() const;
    void setMass(float newMass);

    void setTexture(const std::shared_ptr<sf::Texture> &texture);

private:
    std::shared_ptr<sf::Texture> foodTexture;
    Components::Transform transform;
    Components::Sprite sprite;
    Components::Mass mass;
};