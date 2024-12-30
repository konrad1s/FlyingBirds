#pragma once

#include <cmath>
#include "Component.h"

namespace Components
{
    class Mass : public Component
    {
    public:
        explicit Mass(float initialMass = 10.f)
            : value(initialMass)
        {
        }

        float getValue() const { return value; }

        void addMass(float additional)
        {
            value += additional;
        }

        float getRadius() const
        {
            static constexpr float PI = 3.14159f;
            return std::sqrt(value / PI);
        }

    private:
        float value;
    };
}
