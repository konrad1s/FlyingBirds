#pragma once

#include "Component.h"
#include <SFML/System/Vector2.hpp>

namespace Components
{
    class Position : public Component
    {
    public:
        Position(float x = 0.f, float y = 0.f) : position(x, y) {}

        sf::Vector2f position;
    };

}