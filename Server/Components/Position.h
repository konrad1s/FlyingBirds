#pragma once

#include "Component.h"
#include <SFML/System/Vector2.hpp>

namespace Components
{
    class Position : public Component
    {
    public:
        explicit Position(float x = 0.f, float y = 0.f)
            : coords(x, y)
        {
        }

        sf::Vector2f coords;
    };
}
