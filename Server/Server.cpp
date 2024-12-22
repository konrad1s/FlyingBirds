#include "Server.h"
#include "Logger.h"
#include "Events.h"

Server::Server(const ConfigServer &config, EventBus &eb)
    : config(config), eventBus(eb)
{
}

Server::~Server()
{
    stop();
}

bool Server::start()
{
    if (listener.listen(config.port) != sf::Socket::Done)
    {
        Logger::error("Failed to bind listener to port {}", config.port);
        return false;
    }
    Logger::info("Server listening on port {}", config.port);

    listener.setBlocking(false);

    return true;
}

void Server::stop()
{
    listener.close();
    Logger::info("Server stopped.");
}

void Server::acceptNewClients()
{
    auto newClient = std::make_unique<NetworkHandler>();
    if (listener.accept(newClient->getSocket()) == sf::Socket::Done)
    {
        newClient->getSocket().setBlocking(false);

        ClientId assignedId = nextClientId++;
        clients[assignedId] = std::move(newClient);

        Events::ClientConnectedEvent event{assignedId};
        eventBus.publish(event);

        Logger::info("New client connected with ID: {}", assignedId);
    }
}

void Server::update()
{
    for (auto& [clientId, handlerPtr] : clients)
    {
        receiveFromClient(clientId);
    }
}

void Server::receiveFromClient(ClientId clientId)
{
    sf::Packet packet;
    sf::Socket::Status status = clients[clientId]->getSocket().receive(packet);

    if (status == sf::Socket::Done)
    {
        
    }
    else if (status == sf::Socket::Disconnected)
    {
        
    }
}
