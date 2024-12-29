#include "GameWorld.h"
#include "Client.h"
#include <memory>

class MovementSystem
{
public:
    MovementSystem(std::unique_ptr<Client> &client);

    void update(GameWorld &game, float dt);

private:
    float computeAngleToTarget(float x1, float y1, float x2, float y2);
    uint32_t findClosestPlayer(float x, float y,
                               const std::unordered_map<uint32_t, std::unique_ptr<Player>> &players);
    uint32_t findClosestFood(float x, float y,
                             const std::unordered_map<uint32_t, std::unique_ptr<Food>> &foods);

private:
    std::unique_ptr<Client> &client;
};