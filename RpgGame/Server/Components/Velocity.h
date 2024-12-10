#pragma once
#include "Component.h"

namespace Components
{
    class Velocity : public Component
    {
    public:
        Velocity(float v = 0.f) : velocity(v) {}

        float velocity;
    };
}
