#pragma once
#include "Component.h"

namespace Components
{
    class Speed : public Component
    {
    public:
        float value;

        Speed(float speed = 0.f) : value(speed) {}
    };
}
