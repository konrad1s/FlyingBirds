#pragma once

#include "NetworkHandler.h"
#include <unordered_map>
#include <thread>
#include <future>
#include <string>
#include "Player.h"
#include "Server.h"
#include "ConfigServer.h"
#include "EventBus.h"
#include "network.pb.h"
#include "GameWorld.h"
#include "ScoreboardManager.h"

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

    void run();
    void update(float deltaTime);
    State getState() const { return state; }

private:
    void handlePrompt();
    void checkPrompt();

    void onClientConnected(uint32_t clientId, const std::string &nickname);
    void onClientDisconnected(uint32_t clientId);
    void onClientMessage(uint32_t clientId, const network::ClientToServer &msg);
    void sendWelcomeToClient(uint32_t clientId);
    void broadcastGameState();
    void broadcastScoreboard();
    void broadcastGameStart();
    void handleEndGame(float deltaTime);

private:
    State state = State::waitingForClients;

    std::promise<std::string> promptInput;
    std::future<std::string> promptFuture;
    std::thread promptThread;
    std::atomic<bool> promptThreadRunning{false};

    std::mutex promptMutex;
    EventBus eventBus;
    ScoreboardManager scoreboardManager;
    std::unique_ptr<Server> server;

    float timeRemaining;
    bool isGameOver;

    GameWorld gameWorld;
};