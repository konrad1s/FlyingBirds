#pragma once

#include "Component.h"

namespace Components
{
    class Health : public Component
    {
    public:
        int value;

        Health(int h = 100) : value(h) {}
    };
}
