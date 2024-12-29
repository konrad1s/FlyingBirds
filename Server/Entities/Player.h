#pragma once

#include <cstdint>
#include "Position.h"
#include "Speed.h"
#include "Size.h"

class Player
{
public:
    explicit Player(uint32_t id);

    uint32_t getId() const;

    float getAngle() const;
    void  setAngle(float angle);

    float getX() const;
    float getY() const;
    void  setPosition(float x, float y);

    float getSpeedValue() const;
    void  setSpeedValue(float val);

    float getSizeValue() const;
    void  grow(float amount);

    void update(float deltaTime);

private:
    uint32_t id;
    float angle;

    Components::Position position;
    Components::Speed speed;
    Components::Size size;
};
