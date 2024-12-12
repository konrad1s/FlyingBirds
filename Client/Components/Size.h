#pragma once
#include <algorithm>
#include "Component.h"

class Size : public Component
{
public:
    Size(int _size = 100)
        : size(_size) {}

    int getSize() const { return size; }

    friend Size operator+(const Size &me, int increase)
    {
        return me.size + increase;
    }

    friend Size operator-(const Size &me, int decrease)
    {
        return std::min(me.size - decrease, 0);
    }

private:
    int size;
};