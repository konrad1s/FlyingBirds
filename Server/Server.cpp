#include "Server.h"
#include "Logger.h"
#include <iostream>
#include <future>
#include <thread>

Server::Server(const ConfigServer &_config) : config(_config), nextClientId(0)
{
    game = std::make_unique<GameManager>();
    Logger::info("Server initialized", config.port);
}

void Server::run()
{
    sf::Clock clock;

    if (listener.listen(config.port) != sf::Socket::Done)
    {
        Logger::error("Failed to bind listener to port {}", config.port);
        return;
    }
    Logger::info("Server is listening on port {}", config.port);
    listener.setBlocking(false);

    while (true)
    {
        float deltaTime = clock.restart().asSeconds();

        if (GameManager::State::waitingForClients == game->getState())
        {
            acceptNewClients();
        }

        update(deltaTime);
    }
}

void Server::acceptNewClients()
{
    auto newClient = std::make_unique<NetworkHandler>();

    if (listener.accept(newClient.get()->getSocket()) == sf::Socket::Done)
    {
        clients[nextClientId] = std::move(newClient);
        game->onClientConnected(nextClientId);
        nextClientId++;
    }
}

void Server::update(float deltaTime)
{
    game->update(deltaTime);
}
