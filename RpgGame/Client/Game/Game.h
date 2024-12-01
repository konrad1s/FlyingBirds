#pragma once
#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include "Player.h"

class Game
{
public:
    Game();
    ~Game();
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
};