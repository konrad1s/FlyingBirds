#include "Server.h"
#include <iostream>
#include <future>
#include <thread>

Server::Server(const ConfigServer &_config) : config(_config), nextClientId(0)
{
    game = std::make_unique<GameManager>();
}

void Server::run()
{
    sf::Clock clock;

    if (listener.listen(config.port) != sf::Socket::Done)
    {
        std::cerr << "Failed to bind listener to port " << config.port << std::endl;
        return;
    }
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

    if (listener.accept(newClient.get()->getSocket()))
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
