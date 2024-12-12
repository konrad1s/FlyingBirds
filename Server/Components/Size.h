#pragma once
#include "Component.h"

namespace Components
{
    class Size : public Component
    {
    public:
        Size(int s = 100) : size(s) {}

        int size;
    };
}
