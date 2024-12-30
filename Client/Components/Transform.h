#pragma once
#include "Component.h"

namespace Components
{
    class Transform : public Component
    {
    public:
        float x = 0.0f;
        float y = 0.0f;
    };
}
