#pragma once

#include <cmath>
#include "Component.h"

namespace Components
{
    class Mass : public Component
    {
    public:
        explicit Mass(float initialMass = 100.f)
            : value(initialMass)
        {
        }

        float getValue() const { return value; }
        void  setValue(float m) { value = m; }

        float getRadius() const
        {
            static constexpr float PI = 3.14159f;
            return std::sqrt(value / PI);
        }

        void addMass(float additional)
        {
            value += additional;
        }

    private:
        float value;
    };
}
