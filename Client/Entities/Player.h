#pragma once

#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "Transform.h"
#include "Sprite.h"
#include "Mass.h"

class Player : public Entity
{
public:
    Player();

    void update(float deltaTime) override;
    void render(sf::RenderWindow &window) override;

    void setPosition(float x, float y);
    float getX() const;
    float getY() const;

    float getMass() const;
    void setMass(float newMass);

    void setTexture(sf::Texture *texture);

private:
    Components::Transform transform;
    Components::Sprite sprite;
    Components::Mass mass;

    float speed{100.f};
};