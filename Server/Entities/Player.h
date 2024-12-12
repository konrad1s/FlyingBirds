#pragma once

#include <cstdint>
#include "Size.h"
#include "Velocity.h"
#include "Position.h"

class Player
{
public:
    Player(uint32_t _id);

    uint32_t getId() const;

    Components::Position getPosition() const;
    void setPosition(const Components::Position& pos);

    Components::Velocity getVelocity() const;
    void setVelocity(const Components::Velocity& vel);

    Components::Size getSize() const;

    void grow(int amount);
    void update(float deltaTime);

private:
    uint32_t id;
    Components::Position position;
    Components::Velocity velocity;
    Components::Size size;
};
