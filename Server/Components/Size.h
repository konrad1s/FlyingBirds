#pragma once

#include "Component.h"

namespace Components
{
    class Size : public Component
    {
    public:
        float value;

        Size(float s = 100.f) : value(s) {}
    };
}
