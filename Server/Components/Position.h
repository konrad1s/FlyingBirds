#pragma once

#include <SFML/System/Vector2.hpp>
#include "Component.h"

namespace Components
{
    class Position : public Component
    {
    public:
        sf::Vector2f coords;

        Position(float x = 0.f, float y = 0.f) : coords(x, y) {}
    };
}
