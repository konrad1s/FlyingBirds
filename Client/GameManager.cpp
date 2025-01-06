#include "GameManager.h"
#include "ResourceManager.h"
#include "Logger.h"
#include "Systems.h"

GameManager::GameManager()
    : window(sf::VideoMode(1280, 720), "Game"), hudManager(window, eventBus)
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

    eventBus.subscribe<Events::PlayButtonClickEvent>(
        [this](const Events::PlayButtonClickEvent &evt)
        {
            this->onPlayButtonClicked(evt.ip, evt.port, evt.nickname);
        });

    try
    {
        auto &resMgr = ResourceManager::getInstance();
        backgroundTexture = resMgr.acquire<sf::Texture>("background", "backgrounds/background.png");

        background.setTexture(*backgroundTexture);
        background.setPosition(0.f, 0.f);

        auto textureSize = backgroundTexture->getSize();
        float scaleX = static_cast<float>(1280) / static_cast<float>(textureSize.x);
        float scaleY = static_cast<float>(720) / static_cast<float>(textureSize.y);
        float scale  = std::min(scaleX, scaleY);
        background.setScale(scale, scale);

        float posX = (1280 - textureSize.x * scale) * 0.5f;
        float posY = (720 - textureSize.y * scale) * 0.5f;
        background.setPosition(posX, posY);

    }
    catch (const std::exception &e)
    {
        Logger::error("Failed to load background: {}", e.what());
    }

    gameView.reset(sf::FloatRect(0.f, 0.f, 720.f, 720.f));
    gameView.setViewport(sf::FloatRect(0.f, 0.f, 720.f / 1280.f, 1.f));

    hudView.reset(sf::FloatRect(0.f, 0.f, 560.f, 720.f));
    hudView.setViewport(sf::FloatRect(720.f / 1280.f, 0.f, 560.f / 1280.f, 1.f));
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

        hudManager.handleEvent(event);
    }
}

void GameManager::renderLoop()
{
    window.setActive(true);

    while (isRunning)
    {
        window.clear();

        window.setView(window.getDefaultView());
        window.draw(background);
        {
            std::lock_guard<std::mutex> lock(entityMutex);

            if (world)
            {
                window.setView(gameView);
                world->render(window);

                window.setView(hudView);
                hudManager.render();
            }
            else
            {
                hudManager.render();
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
    MovementSystem movementSystem(client);

    while (isRunning) 
    {
        deltaTime = clock.restart().asSeconds();

        {
            std::lock_guard<std::mutex> lock(entityMutex);
            if (world)
            {
                world->update(deltaTime);
                movementSystem.update(*world, deltaTime);
                hudManager.update(*world, deltaTime);
            }
            else
            {
                hudManager.update(deltaTime);
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void GameManager::onServerWelcome(const Events::WelcomeEvent &evt)
{
    if (!evt.message.entities().empty())
    {
        hudManager.setState(HUDManager::State::InGame);
        world = std::make_unique<GameWorld>();

        auto &player = evt.message.entities().Get(0);

        world->setMyPlayerId(player.id());
        Logger::info("Server welcome received, player id {}", player.id());
    }
    else
    {
        Logger::warning("Server welcome received, but players list is empty.");
    }
}

void GameManager::onServerDataUpdate(const Events::StateUpdateEvent &evt)
{
    {
        std::lock_guard<std::mutex> lock(entityMutex);
        if (world)
        {
            world->updateFromServer(evt.message);
        }
    }
}

void GameManager::onPlayButtonClicked(const std::string &ip, unsigned short port, const std::string &nick)
{
    if (!ip.empty() && port != 0)
    {
        client = std::make_unique<Client>(sf::IpAddress(ip), port, eventBus);
        if (0 == client->start())
        {
            network::ClientToServer joinMsg;
            joinMsg.set_type(network::ClientToServer::JOIN);
            joinMsg.set_nickname(nick);
            client->sendToServer(joinMsg);
        }
    }
    else
    {
        Logger::warning("Invalid IP or Port entered.");
    }
}
