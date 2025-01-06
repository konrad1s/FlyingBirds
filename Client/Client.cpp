#include "Client.h"
#include "Logger.h"
#include "Events.h"

Client::Client(const sf::IpAddress &serverIp, unsigned short serverPort, EventBus &eventBus) : serverIp(serverIp), serverPort(serverPort), eventBus(eventBus)
{
}

Client::~Client()
{
}

int Client::start()
{
    if (networkHandler.getSocket().connect(serverIp, serverPort) != sf::Socket::Done)
    {
        Logger::error("Failed to connect to the server :{}", serverPort);
        return -1;
    }

    Logger::info("Connected to the server :{}", serverPort);
    running = true;
    networkThread = std::thread(&Client::networkLoop, this);

    return 0;
}

void Client::stop()
{
    if (!running.exchange(false))
    {
        /* Already stopped */
        return;
    }

    networkHandler.getSocket().disconnect();

    if (networkThread.joinable())
        networkThread.join();

    Logger::info("Disconnected from the server");
}

void Client::networkLoop()
{
    while (running)
    {
        network::Envelope envelope;
        int result = networkHandler.receiveMessage<network::Envelope>(envelope);

        if (result == 0)
        {
            if (envelope.category() == network::Envelope::SERVER_TO_CLIENT)
            {
                auto msgType = envelope.s2c().type();

                switch (msgType)
                {
                case network::ServerToClient::STATE_UPDATE:
                {
                    Events::StateUpdateEvent event{envelope.s2c()};
                    eventBus.publish<Events::StateUpdateEvent>(event);
                }
                break;
                case network::ServerToClient::WELCOME:
                {
                    Events::WelcomeEvent event{envelope.s2c()};
                    eventBus.publish<Events::WelcomeEvent>(event);
                }
                break;
                case network::ServerToClient::GAME_START:
                {
                    Events::GameStartEvent event{};
                    eventBus.publish<Events::GameStartEvent>(event);
                }
                break;
                case network::ServerToClient::GOODBYE:
                {
                    Events::GoodbyeEvent event{envelope.s2c()};
                    eventBus.publish<Events::GoodbyeEvent>(event);
                }
                break;
                default:
                {
                    Logger::warning("Received unknown ServerToClient message");
                }
                break;
                }
            }
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}

int Client::sendToServer(const network::ClientToServer &msg)
{
    network::Envelope envelope;

    envelope.set_category(network::Envelope::CLIENT_TO_SERVER);
    *envelope.mutable_c2s() = msg;

    if (networkHandler.sendMessage<network::Envelope>(envelope) != 0)
    {
        Logger::error("Failed to send message to server");
        return -1;
    }

    return 0;
}
