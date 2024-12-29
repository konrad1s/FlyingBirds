#pragma once
#include <cmath>

namespace AngleUtils
{
    constexpr float PI = 3.14159265358979323846f;

    inline float degreesToRadians(float degrees)
    {
        return degrees * (PI / 180.f);
    }

    inline float radiansToDegrees(float radians)
    {
        return radians * (180.f / PI);
    }
}