#pragma once

#include "Component.h"

namespace Components
{
    class Speed : public Component
    {
    public:
        explicit Speed(float s = 0.f) : value(s) {}
        float value;
    };
}
