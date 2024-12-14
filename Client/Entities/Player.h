#pragma once

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

private:
    const int speed = 100;

    Components::Health health;
    Components::Sprite sprite;
    Components::Transform transform;
};