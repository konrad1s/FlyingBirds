#include "Client.h"
#include "Logger.h"

Client::Client(const sf::IpAddress &serverIp, unsigned short serverPort) : serverIp(serverIp), serverPort(serverPort)
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
    networkThread = std::thread(&Client::networkLoop, this);

    return 0;
}

void Client::stop()
{
    networkHandler.getSocket().disconnect();

    if (networkThread.joinable())
        networkThread.join();

    Logger::info("Disconnected from the server");
}

void Client::networkLoop()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}
