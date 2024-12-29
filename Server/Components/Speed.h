#pragma once
#include "Component.h"

namespace Components
{
    class Speed : public Component
    {
    public:
        Speed(float speed = 0.f) : speed(speed) {}

        float speed;
    };
}
