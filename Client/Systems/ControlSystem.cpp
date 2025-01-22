#include "ControlSystem.h"
#include "GameWorld.h"
#include "Player.h"
#include "Entity.h"
#include "Logger.h"
#include "AngleUtils.h"
#include <cmath>
#include <limits>

ControlSystem::ControlSystem(const std::string &luaScriptPath)
{
    lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::table, sol::lib::string);

    sol::load_result script = lua.load_file(luaScriptPath);
    if (!script.valid())
    {
        sol::error err = script;
        Logger::error("Failed to load Lua script '{}': {}", luaScriptPath, err.what());
        return;
    }

    sol::protected_function_result result = script();
    if (!result.valid())
    {
        sol::error err = result;
        Logger::error("Runtime error in Lua script '{}': {}", luaScriptPath, err.what());
        return;
    }

    /* Fetch the function getAngle(myId, players, entities) */
    getAngleFunc = lua["getAngle"];
    if (!getAngleFunc.valid())
    {
        Logger::warning("Lua script does not define 'getAngle' function");
    }
}

void ControlSystem::update(Client &client, GameWorld &world, uint32_t myPlayerId)
{
    if (!getAngleFunc.valid())
        return;

    const auto &players = world.getPlayers();
    auto it = players.find(myPlayerId);
    if (it == players.end())
    {
        return;
    }

    sol::table playersTable = createPlayersTable(lua, world);
    sol::table entitiesTable = createEntitiesTable(lua, world);

    sol::protected_function_result luaResult = getAngleFunc(myPlayerId, playersTable, entitiesTable);

    if (!luaResult.valid())
    {
        sol::error err = luaResult;
        Logger::error("Error calling Lua getAngle: {}", err.what());
        return;
    }

    float angleDegrees = luaResult.get<float>();

    while (angleDegrees < 0.f)
        angleDegrees += 360.f;
    while (angleDegrees >= 360.f)
        angleDegrees -= 360.f;

    network::ClientToServer moveMsg;
    moveMsg.set_type(network::ClientToServer::MOVE);
    moveMsg.set_angle(angleDegrees);
    client.sendToServer(moveMsg);
}

sol::table ControlSystem::createPlayersTable(sol::state &lua, const GameWorld &world)
{
    sol::table playersTable = lua.create_table();

    for (auto &[id, playerPtr] : world.getPlayers())
    {
        sol::table p = lua.create_table();
        p["id"] = id;
        p["x"] = playerPtr->getX();
        p["y"] = playerPtr->getY();
        p["mass"] = playerPtr->getMass();
        p["nickname"] = playerPtr->getNickname();
        p["speedBoost"] = playerPtr->isSpeedBoostActive();
        p["protection"] = playerPtr->isProtectionActive();
        p["attacking"] = playerPtr->isAttacking();

        playersTable[id] = p;
    }

    return playersTable;
}

sol::table ControlSystem::createEntitiesTable(sol::state &lua, const GameWorld &world)
{
    sol::table entitiesTable = lua.create_table();

    for (auto &[id, entityPtr] : world.getEntities())
    {
        sol::table e = lua.create_table();
        e["id"] = id;
        e["x"] = entityPtr->getX();
        e["y"] = entityPtr->getY();

        if (auto food = dynamic_cast<Food *>(entityPtr.get()))
        {
            e["mass"] = food->getMass();
            e["type"] = "food";
        }
        else if (dynamic_cast<SpeedBoost *>(entityPtr.get()))
        {
            e["mass"] = 0.f;
            e["type"] = "speedboost";
        }
        else if (dynamic_cast<Protection *>(entityPtr.get()))
        {
            e["mass"] = 0.f;
            e["type"] = "protection";
        }
        else
        {
            e["mass"] = 0.f;
            e["type"] = "unknown";
        }

        entitiesTable[id] = e;
    }

    return entitiesTable;
}
