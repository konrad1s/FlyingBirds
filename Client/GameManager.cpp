#include "GameManager.h"
#include "ResourceManager.h"
#include "Logger.h"

GameManager::GameManager() : window(sf::VideoMode(800, 800), "Game"), isRunning(false)
{
    eventBus.subscribe<Events::WelcomeEvent>(
        [this](const Events::WelcomeEvent &evt)
        {
            this->onServerWelcome(evt);
        });

    eventBus.subscribe<Events::StateUpdateEvent>(
        [this](const Events::StateUpdateEvent &evt)
        {
            this->onServerDataUpdate(evt);
        });
}

GameManager::~GameManager()
{
    isRunning = false;

    if (renderThread.joinable())
        renderThread.join();
    if (updateThread.joinable())
        updateThread.join();
    if (client)
        client->stop();
}

void GameManager::run()
{
    isRunning = true;
    window.setActive(false);
    renderThread = std::thread(&GameManager::renderLoop, this);
    updateThread = std::thread(&GameManager::updateLoop, this);

    world = std::make_unique<GameWorld>();

    client = std::make_unique<Client>(sf::IpAddress::LocalHost, 5000, eventBus);
    client->start();

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
            if (world)
            {
                world->render(window);
            }
        }

        window.display();

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
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
            if (world)
            {
                world->update(deltaTime);
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

void GameManager::onServerWelcome(const Events::WelcomeEvent &evt)
{
    Logger::info("Server welcome received");
}

void GameManager::onServerDataUpdate(const Events::StateUpdateEvent &evt)
{
    Logger::info("Data update received");

    {
        std::lock_guard<std::mutex> lock(entityMutex);
        if (world)
        {
            world->updateFromServer(evt.message);
        }
    }
}
