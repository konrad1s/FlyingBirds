#pragma once

#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "Transform.h"
#include "Sprite.h"
#include "Mass.h"
#include "Animation.h"

class Player : public Entity
{
public:
    Player();

    void update(float deltaTime) override;
    void render(sf::RenderWindow &window) override;

    float getMass() const;
    void setMass(float newMass);

    Components::Animation &getAnimation();
    void setTexture(const std::shared_ptr<sf::Texture> &texture);
    const std::shared_ptr<sf::Texture> &getTexture() const;

private:
    Components::Mass mass;
    Components::Animation animation;
    std::shared_ptr<sf::Texture> defaultTexture;

    float speed{100.f};
    float lastX;
};