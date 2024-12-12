#pragma once

#include "ClientConnection.h"
#include <unordered_map>
#include <thread>
#include <future>
#include <string>
#include "Player.h"

class GameManager
{
public:
    enum class State
    {
        waitingForClients,
        starting,
        running,
        finished
    };

    GameManager();
    ~GameManager();

    void update(float deltaTime);
    void checkPrompt();

    State getState();

    void onClientConnected(uint32_t clientId);
    void onClientDisconnected(uint32_t clientId);
    void handleClientMessage(uint32_t clientId);
    void serializeGameData();

private:
    void handlePrompt();

    std::promise<std::string> promptInput;
    std::future<std::string> promptFuture;
    std::thread promptThread;
    bool promptThreadRunning = false;

    std::unordered_map<uint32_t, std::unique_ptr<Player>> clients;
    State state;
};