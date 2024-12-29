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
    network::Envelope envelope;
    int result = clients[clientId]->receiveMessage<network::Envelope>(envelope);

    if (result == 0)
    {
        if (envelope.category() == network::Envelope::CLIENT_TO_SERVER)
        {
            auto msgType = envelope.c2s().type();

            switch (msgType)
            {
            case network::ClientToServer::MOVE:
            {
                Events::ClientDataUpdatedEvent event{clientId, envelope.c2s()};
                eventBus.publish<Events::ClientDataUpdatedEvent>(event);
            }
            break;
            case network::ClientToServer::JOIN:
            {
                Events::ClientConnectedEvent event{clientId};
                eventBus.publish<Events::ClientConnectedEvent>(event);
            }
            break;
            case network::ClientToServer::QUIT:
            {
                Events::ClientDisconnectedEvent event{clientId};
                eventBus.publish<Events::ClientDisconnectedEvent>(event);
            }
            break;
            default:
            {
                Logger::warning("Received unknown ClientToServer message");
            }
            break;
            }
        }
    }
}
