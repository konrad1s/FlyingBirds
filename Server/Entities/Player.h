#pragma once

#include <cstdint>
#include "Size.h"
#include "Speed.h"
#include "Position.h"

class Player
{
public:
    Player(uint32_t _id);

    uint32_t getId() const;

    Components::Position getPosition() const;
    void setPosition(const Components::Position& pos);

    Components::Speed getSpeed() const;
    void setSpeed(const Components::Speed& vel);

    float getAngle() const;
    void setAngle(const float angle);

    Components::Size getSize() const;

    void grow(int amount);
    void update(float deltaTime);

private:
    uint32_t id;
    float angle;
    Components::Position position;
    Components::Speed speed;
    Components::Size size;
};
