#pragma once

#include <sol/sol.hpp>
#include <memory>
#include <string>
#include "ISystem.h"
#include "Client.h"

class ControlSystem : public ISystem
{
public:
    ControlSystem(const std::string &luaScriptPath);

    void update(GameWorld &world, float deltaTime) override {}
    void update(Client &client, GameWorld &world, uint32_t myPlayerId);

private:
    sol::table createPlayersTable(sol::state &lua, const GameWorld &world);
    sol::table createEntitiesTable(sol::state &lua, const GameWorld &world);

private:
    sol::state lua;

    sol::function getAngleFunc;
};
