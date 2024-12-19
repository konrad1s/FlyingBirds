#include "GameManager.h"
#include "ResourceManager.h"
#include "Client.h"

GameManager::GameManager() : window(sf::VideoMode(800, 600), "Game"), isRunning(false)
{
}

GameManager::~GameManager()
{
    isRunning = false;

    if (renderThread.joinable())
        renderThread.join();
    if (updateThread.joinable())
        updateThread.join();
}

void GameManager::run()
{
    isRunning = true;
    window.setActive(false);
    renderThread = std::thread(&GameManager::renderLoop, this);
    updateThread = std::thread(&GameManager::updateLoop, this);

    static Client client = Client(sf::IpAddress::LocalHost, 5000);
    client.start();

    while (window.isOpen())
    {
        handleEvents();
    }
}

void GameManager::handleEvents()
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

void GameManager::renderLoop()
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

void GameManager::updateLoop()
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
