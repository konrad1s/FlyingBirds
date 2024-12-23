#pragma once
#include "Component.h"

namespace Components
{
    class Size : public Component
    {
    public:
        Size(float s = 100) : size(s) {}

        float size;
    };
}
