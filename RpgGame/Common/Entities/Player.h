#pragma once

#include "Entity.h"
#include "HealthComponent.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"

class Player : public Entity
{
public:
    Player();
    void update(float deltaTime) override;
    void render(sf::RenderWindow &window) override;

private:
    const int speed = 100;

    HealthComponent health;
    SpriteComponent sprite;
    TransformComponent transform;
};