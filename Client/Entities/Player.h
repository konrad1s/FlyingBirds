#pragma once

#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "Health.h"
#include "Sprite.h"
#include "Transform.h"

class Player : public Entity
{
public:
    Player();
    void update(float deltaTime) override;
    void render(sf::RenderWindow &window) override;

    void setPosition(float x, float y);
    void setHealth(int hp);
    void setTexture(sf::Texture* texture);

    const Components::Transform& getTransform() const { return transform; }

private:
    const int speed = 100;

    Components::Health health;
    Components::Sprite sprite;
    Components::Transform transform;
};
