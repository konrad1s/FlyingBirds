#pragma once
#include <string>
#include <thread>
#include <mutex>
#include "NetworkHandler.h"

class Client
{
public:
    Client(const sf::IpAddress &serverIp, unsigned short serverPort);
    ~Client();

    int start();
    void stop();

private:
    void networkLoop();

    // EventBus &eventBus;
    sf::IpAddress serverIp;
    unsigned short serverPort;
    NetworkHandler networkHandler;
    std::thread networkThread;
};
