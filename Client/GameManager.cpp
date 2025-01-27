#include "GameManager.h"
#include "ResourceManager.h"
#include "Logger.h"
#include "Systems.h"

GameManager::GameManager()
    : window(sf::VideoMode(1280, 720), "FlyingBirds"), hudManager(window, eventBus)
{
    eventBus.subscribe<Events::WelcomeEvent>(
        [this](const Events::WelcomeEvent &evt)
        {
            this->onServerWelcome(evt);
        });
    eventBus.subscribe<Events::GameStartEvent>(
        [this](const Events::GameStartEvent &evt)
        {
            this->onGameStarted();
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
    eventBus.subscribe<Events::GoodbyeEvent>(
        [this](const Events::GoodbyeEvent &evt)
        {
            this->onServerGoodbye(evt);
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

        auto iconTexture = resMgr.acquire<sf::Texture>("icon", "hud/icon.png");
        if (iconTexture)
        {
            sf::Image iconImage = iconTexture->copyToImage();
            window.setIcon(iconImage.getSize().x, iconImage.getSize().y, iconImage.getPixelsPtr());
        }
        else
        {
            Logger::error("Failed to acquire icon texture.");
        }
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
    ControlSystem movementSystem("../../../Client/Systems/user_algo.lua");

    while (isRunning) 
    {
        deltaTime = clock.restart().asSeconds();

        {
            std::lock_guard<std::mutex> lock(entityMutex);
            if (world)
            {
                world->update(deltaTime);
                movementSystem.update(*client, *world, myPlayerId);
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
        auto &player = evt.message.entities().Get(0);

        myPlayerId = player.id();
        Logger::info("Server welcome received, player id {}", player.id());

        hudManager.setMenuStatus(MenuHUD::Status::Connected_Waiting);
    }
    else
    {
        Logger::warning("Server welcome received, but players list is empty.");

        hudManager.setMenuStatus(MenuHUD::Status::Error);
    }
}

void GameManager::onGameStarted()
{
    hudManager.setState(HUDManager::State::InGame);
    world = std::make_unique<GameWorld>();
    Logger::info("Game started received");
}

void GameManager::onServerDataUpdate(const Events::StateUpdateEvent &evt)
{
    {
        std::lock_guard<std::mutex> lock(entityMutex);
        if (world)
        {
            world->updateFromServer(evt.message);
        }

        float serverTimeRemaining = evt.message.time_remaining();
        hudManager.setTimeRemaining(serverTimeRemaining);
    }
}

void GameManager::onPlayButtonClicked(const std::string &ip, unsigned short port, const std::string &nick)
{
    if (!ip.empty() && port != 0)
    {
        hudManager.setMenuStatus(MenuHUD::Status::Connecting);

        client = std::make_unique<Client>(sf::IpAddress(ip), port, eventBus);
        if (0 == client->start())
        {
            network::ClientToServer joinMsg;
            joinMsg.set_type(network::ClientToServer::JOIN);
            joinMsg.set_nickname(nick);
            client->sendToServer(joinMsg);
        }
        else
        {
            client.reset();
            Logger::error("Failed to start client.");
            hudManager.setMenuStatus(MenuHUD::Status::InvalidIPPort);
        }
    }
    else
    {
        hudManager.setMenuStatus(MenuHUD::Status::InvalidIPPort);
        Logger::warning("Invalid IP or Port entered.");
    }
}

void GameManager::onServerGoodbye(const Events::GoodbyeEvent &evt)
{
    std::vector<GameEndHUD::ScoreEntry> finalScores;
    finalScores.reserve(evt.message.entities().size());

    std::lock_guard<std::mutex> lock(entityMutex);
    for (int i = 0; i < evt.message.entities().size(); ++i)
    {
        const auto &netEnt = evt.message.entities().Get(i);

        GameEndHUD::ScoreEntry entry;
        entry.playerId = netEnt.id();
        entry.mass     = netEnt.mass();

        if (world)
        {
            const auto &players = world->getPlayers();
            auto it = players.find(netEnt.id());
            if (it != players.end() && it->second)
            {
                entry.nickname = it->second->getNickname();
                entry.playerTexture = it->second->getTexture();
            }
        }

        if (entry.nickname.empty())
        {
            entry.nickname = "Player " + std::to_string(netEnt.id());
        }

        finalScores.push_back(entry);
    }

    hudManager.setState(HUDManager::State::GameEnd);

    if (auto gameEndHUD = dynamic_cast<GameEndHUD*>(hudManager.getHUD().get()))
    {
        gameEndHUD->setScoreboard(finalScores);
    }

    world.reset();

    Logger::info("Server goodbye received, final scoreboard sent.");
}
