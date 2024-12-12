#pragma once
#include "Component.h"

namespace Components
{
    class Health : public Component
    {
    public:
        Health(int h = 100) : health(h) {}

        int health;
    };
}
