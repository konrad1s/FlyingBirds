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

    Health health;
    Sprite sprite;
    Transform transform;
};