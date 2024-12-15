#pragma once

#include <unordered_map>
#include "GameManager.h"
#include "ConfigServer.h"
#include "NetworkHandler.h"

class Server
{
public:
    Server(const ConfigServer &_config);

    void run();

private:
    typedef uint32_t ClientId;

    void acceptNewClients();
    void update(float deltaTime);

    const ConfigServer &config;
    std::unique_ptr<GameManager> game;
    sf::TcpListener listener;
    std::unordered_map<ClientId, std::unique_ptr<NetworkHandler>> clients;
    ClientId nextClientId;
};