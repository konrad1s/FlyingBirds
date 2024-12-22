#pragma once
#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include "Player.h"
#include "GameWorld.h"
#include "EventBus.h"
#include "Client.h"
#include "Events.h"

class GameManager
{
public:
    GameManager();
    ~GameManager();
    void run();

private:
    void renderLoop();
    void updateLoop();
    void handleEvents();

    void onServerWelcome(const Events::WelcomeEvent &evt);

private:
    bool isRunning;
    Player player;
    EventBus eventBus;
    sf::RenderWindow window;
    std::thread renderThread;
    std::thread updateThread;
    std::mutex entityMutex;
    std::unique_ptr<Client> client;
    std::unique_ptr<GameWorld> world;
};