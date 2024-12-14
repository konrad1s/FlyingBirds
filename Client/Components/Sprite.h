#pragma once

#include "Component.h"
#include <SFML/Graphics.hpp>

namespace Components
{
    class Sprite : public Component
    {
    public:
        Sprite() {}

    public:
        sf::Sprite sprite;
    };
}