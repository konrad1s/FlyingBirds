#pragma once
#include <algorithm>
#include "Component.h"

class SizeComponent : public Component
{
public:
    SizeComponent(int _size = 100)
        : size(_size) {}

    int getSize() const { return size; }

    friend SizeComponent operator+(const SizeComponent &me, int increase)
    {
        return me.size + increase;
    }

    friend SizeComponent operator-(const SizeComponent &me, int decrease)
    {
        return std::min(me.size - decrease, 0);
    }

private:
    int size;
};