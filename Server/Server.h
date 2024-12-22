#pragma once

#include <unordered_map>
#include "ConfigServer.h"
#include "NetworkHandler.h"
#include "EventBus.h"
#include "Logger.h"

class Server
{
public:
    using ClientId = uint32_t;

    Server(const ConfigServer &config, EventBus &eb);
    ~Server();

    bool start();
    void stop();

    template <typename MsgType>
    void sendToClient(ClientId clientId, const MsgType &msg);

    template <typename MsgType>
    void broadcast(const MsgType &msg);

    void acceptNewClients();
    void update();

private:
    void receiveFromClient(ClientId clientId);

    const ConfigServer &config;
    EventBus &eventBus;

    sf::TcpListener listener;
    ClientId nextClientId = 0;

    std::unordered_map<ClientId, std::unique_ptr<NetworkHandler>> clients;
};

#include "Server.ipp"
