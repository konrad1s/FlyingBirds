#include "MovementSystem.h"
#include "ConfigServer.h"
#include "Player.h"

void MovementSystem::update(GameWorld &world, float deltaTime)
{
    for (auto &[id, playerPtr] : world.getPlayers())
    {
        playerPtr->update(deltaTime, ConfigServer::worldSize, ConfigServer::worldSize);
    }
}
