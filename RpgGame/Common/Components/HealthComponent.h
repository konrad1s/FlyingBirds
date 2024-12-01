#pragma once
#include <algorithm>
#include "Component.h"

class HealthComponent : public Component
{
public:
    HealthComponent(int _current = 0, int _max = 0)
        : current(std::clamp(_current, 0, _max)), max(std::max(0, _max)) {}

    int getHp() const { return current; }
    int getMaxHp() const { return max; }
    bool isAlive() const { return current > 0; }

    friend HealthComponent operator+(const HealthComponent &me, int hp)
    {
        return std::min(me.current + hp, me.max);
    }

    friend HealthComponent operator-(const HealthComponent &me, int hp)
    {
        return std::max(me.current - hp, 0);
    }

private:
    int current;
    int max;
};