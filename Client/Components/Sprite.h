#pragma once

#include <SFML/Graphics.hpp>
#include "Component.h"

namespace Components
{
    class Sprite : public Component
    {
    public:
        sf::Sprite sprite; 
    };
}
