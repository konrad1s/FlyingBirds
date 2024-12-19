#pragma once
#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include "Player.h"
#include "GameWorld.h"

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

private:
    bool isRunning;
    Player player;
    sf::RenderWindow window;
    std::thread renderThread;
    std::thread updateThread;
    std::mutex entityMutex;
    std::unique_ptr<GameWorld> world;
};