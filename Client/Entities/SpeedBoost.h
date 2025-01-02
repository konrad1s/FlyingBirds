
#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "Entity.h"
#include "Sprite.h"

class SpeedBoost : public Entity
{
public:
    SpeedBoost() = default;

    void update(float deltaTime) override;
    void render(sf::RenderWindow &window) override;

    void setTexture(const std::shared_ptr<sf::Texture> &texture);

private:
    Components::Sprite sprite;
    std::shared_ptr<sf::Texture> texture;
};