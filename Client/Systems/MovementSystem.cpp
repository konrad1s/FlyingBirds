#include "MovementSystem.h"
#include <cmath>

MovementSystem::MovementSystem(std::unique_ptr<Client> &client)
    : client(client)
{
}

void MovementSystem::update(GameWorld &game, float dt)
{
    /* TODO: Implement the algorithm */
    float angle = 90;

    network::ClientToServer moveMsg;
    moveMsg.set_type(network::ClientToServer::MOVE);
    moveMsg.set_angle(angle);
    client->sendToServer(moveMsg);
}

float MovementSystem::computeAngleToTarget(float x1, float y1, float x2, float y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;

    return std::atan2(dy, dx);
}

uint32_t MovementSystem::findClosestPlayer(float x, float y,
                                           const std::unordered_map<uint32_t, std::unique_ptr<Player>> &players)
{
    return 0;
}

uint32_t MovementSystem::findClosestFood(float x, float y,
                                         const std::unordered_map<uint32_t, std::unique_ptr<Food>> &foods)
{
    return 0;
}