#include "MovementSystem.h"
#include "GameWorld.h"
#include "Player.h"
#include "AngleUtils.h"
#include "Logger.h"
#include <cmath>

MovementSystem::MovementSystem(std::unique_ptr<Client> &client)
    : client(client)
{
}

void MovementSystem::update(GameWorld &game, float dt)
{
    uint32_t myId = game.getMyPlayerId();
    const auto &players = game.getPlayers();

    auto it = players.find(myId);
    if (it == players.end())
    {
        Logger::warning("Player id: {} not found", myId);
        return;
    }

    float playerX = it->second->getTransform().x;
    float playerY = it->second->getTransform().y;

    uint32_t closestFoodId = findClosestFood(playerX, playerY, game.getFoods());

    if (closestFoodId == 0)
    {
        return;
    }

    const auto &closestFood = game.getFoods().at(closestFoodId);
    float foodX = closestFood->getX();
    float foodY = closestFood->getY();

    float angleRadians = computeAngleToTarget(playerX, playerY, foodX, foodY);
    float angleDegrees = AngleUtils::radiansToDegrees(angleRadians);

    if (angleDegrees < 0)
    {
        angleDegrees += 360.f;
    }

    network::ClientToServer moveMsg;
    moveMsg.set_type(network::ClientToServer::MOVE);
    moveMsg.set_angle(angleDegrees);
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
    uint32_t closestFoodId = 0;
    float minDistanceSquared = std::numeric_limits<float>::max();

    for (const auto &[id, food] : foods)
    {
        float dx = food->getX() - x;
        float dy = food->getY() - y;
        float distanceSquared = dx * dx + dy * dy;

        if (distanceSquared < minDistanceSquared)
        {
            minDistanceSquared = distanceSquared;
            closestFoodId = id;
        }
    }

    return closestFoodId;
}