#include "MovementSystem.h"
#include "GameWorld.h"
#include "Entity.h"
#include "ConfigServer.h"

void MovementSystem::update(GameWorld &world, float deltaTime)
{
    for (auto &[id, playerPtr] : world.getPlayers())
    {
        playerPtr->update(deltaTime, ConfigServer::worldSize, ConfigServer::worldSize);
    }
}
