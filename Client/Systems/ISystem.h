#pragma once

class GameWorld;

class ISystem
{
public:
    virtual ~ISystem() = default;
    virtual void update(GameWorld &world, float deltaTime) = 0;
};
