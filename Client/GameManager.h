#pragma once
#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include "Player.h"
#include "GameWorld.h"
#include "EventBus.h"
#include "Client.h"
#include "Events.h"
#include "MenuHUD.h"
#include "InGameHUD.h"
#include "HUDManager.h"

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
    void onGameStarted();
    void onServerDataUpdate(const Events::StateUpdateEvent &evt);
    void onPlayButtonClicked(const std::string &ip, unsigned short port, const std::string &nick);
    void onServerGoodbye(const Events::GoodbyeEvent &evt);

private:
    bool isRunning{false};
    uint32_t myPlayerId{0};

    EventBus eventBus;
    sf::RenderWindow window;
    std::thread renderThread;
    std::thread updateThread;
    std::mutex entityMutex;
    std::unique_ptr<Client> client;
    std::unique_ptr<GameWorld> world;

    sf::Sprite background;
    std::shared_ptr<sf::Texture> backgroundTexture;

    sf::View gameView;
    sf::View hudView;

    HUDManager hudManager;
};