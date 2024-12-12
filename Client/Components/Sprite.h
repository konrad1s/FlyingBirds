#pragma once

#include "Component.h"
#include <SFML/Graphics.hpp>

class Sprite : public Component
{
public:
    Sprite() {}

public:
    sf::Sprite sprite;
};
