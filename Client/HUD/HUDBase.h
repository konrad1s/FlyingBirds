#pragma once
#include <SFML/Graphics.hpp>

class HUDBase {
public:
    virtual ~HUDBase() = default;

    virtual void handleEvent(sf::RenderWindow &window, const sf::Event &event) = 0;

    virtual void update(float deltaTime) = 0;

    virtual void render(sf::RenderWindow& window) = 0;
};
