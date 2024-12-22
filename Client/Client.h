#pragma once
#include <string>
#include <thread>
#include <mutex>
#include <atomic>
#include "NetworkHandler.h"
#include "EventBus.h"
#include "network.pb.h"

class Client
{
public:
    Client(const sf::IpAddress &serverIp, unsigned short serverPort, EventBus &eventBus);
    ~Client();

    int start();
    void stop();

    int sendToServer(const network::ClientToServer &msg);

private:
    void networkLoop();

    sf::IpAddress serverIp;
    unsigned short serverPort;

    EventBus &eventBus;
    NetworkHandler networkHandler;

    std::atomic<bool> running{false};
    std::thread networkThread;
};
