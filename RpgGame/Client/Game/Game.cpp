#include "Game.h"
#include "ResourceManager.h"

Game::Game() : window(sf::VideoMode(800, 600), "Game"), isRunning(false)
{
}

Game::~Game()
{
    isRunning = false;

    if (renderThread.joinable())
        renderThread.join();
    if (updateThread.joinable())
        updateThread.join();
}

void Game::run()
{
    isRunning = true;
    window.setActive(false);
    renderThread = std::thread(&Game::renderLoop, this);
    updateThread = std::thread(&Game::updateLoop, this);

    while (window.isOpen())
    {
        handleEvents();
    }
}

void Game::handleEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
            isRunning = false;
        }
    }
}

void Game::renderLoop()
{
    window.setActive(true);

    while (isRunning)
    {
        window.clear();

        {
            std::lock_guard<std::mutex> lock(entityMutex);
            player.render(window);
        }

        window.display();

        std::this_thread::sleep_for(std::chrono::milliseconds(60));
    }
}

void Game::updateLoop()
{
    static float deltaTime;
    sf::Clock clock;

    while (isRunning) 
    {
        deltaTime = clock.restart().asSeconds();

        {
            std::lock_guard<std::mutex> lock(entityMutex);
            player.update(deltaTime);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
