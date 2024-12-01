#pragma once

#include "Component.h"
#include <SFML/Graphics.hpp>

class SpriteComponent : public Component
{
public:
    SpriteComponent() {}

public:
    sf::Sprite sprite;
};
