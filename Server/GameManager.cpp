#include <iostream>
#include "GameManager.h"
#include "Logger.h"
#include "Events.h"

GameManager::GameManager(const ConfigServer &config)
    : state(State::waitingForClients)
{
    promptFuture = promptInput.get_future();
    server = std::make_unique<Server>(config, eventBus);

    eventBus.subscribe<Events::ClientConnectedEvent>(
        [this](const Events::ClientConnectedEvent &e)
        {
            this->onClientConnected(e.clientId);
        });

    promptThreadRunning = true;
    promptThread = std::thread(&GameManager::handlePrompt, this);
}

GameManager::~GameManager()
{
    if (promptThread.joinable())
        promptThread.join();
}

void GameManager::run()
{
    if (!server->start())
    {
        return;
    }

    sf::Clock clock;

    while (true)
    {
        float deltaTime = clock.restart().asSeconds();

        if (state == State::waitingForClients)
        {
            server->acceptNewClients();
        }

        update(deltaTime);
        server->update();

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void GameManager::update(float deltaTime)
{
    if (state == State::waitingForClients)
    {
        checkPrompt();
    }
    else if (state == State::starting)
    {
        if (promptThread.joinable())
            promptThread.join();
        promptThreadRunning = false;

        state = State::running;
        Logger::info("GameManager state changed to running.");
    }
    else if (state == State::running)
    {
        /* TODO: Handle running state */
    }
    else if (state == State::finished)
    {
        /* TODO: Handle finished state */
    }
}

void GameManager::checkPrompt()
{
    if (promptFuture.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready)
    {
        try
        {
            std::string input = promptFuture.get();
            Logger::info("Received prompt input: {}", input);
            if (input == "START")
            {
                state = State::starting;
                Logger::info("GameManager starting.");
            }

            {
                std::lock_guard<std::mutex> lock(promptMutex);
                promptInput = std::promise<std::string>();
                promptFuture = promptInput.get_future();
            }

            if (!promptThreadRunning && state == State::waitingForClients)
            {
                promptThreadRunning = true;
                promptThread = std::thread(&GameManager::handlePrompt, this);
            }
        }
        catch (const std::exception& e)
        {
            Logger::error("Error handling prompt input: {}", e.what());
        }
    }
}

void GameManager::onClientConnected(uint32_t clientId)
{
    auto playerPtr = std::make_unique<Player>(clientId);
    clients[clientId] = std::move(playerPtr);
    Logger::info("Player {} connected to the game.", clientId);
}

void GameManager::onClientDisconnected(uint32_t clientId)
{
    if (clients.find(clientId) != clients.end())
    {
        clients.erase(clientId);
        Logger::info("Player {} disconnected from the game.", clientId);
    }
}

void GameManager::onClientMessage(uint32_t clientId, const network::ClientToServer &msg)
{
}

void GameManager::sendWelcomeToClient(uint32_t clientId)
{
    network::Envelope envelope;
    envelope.set_category(network::Envelope::SERVER_TO_CLIENT);

    network::ServerToClient *s2c = envelope.mutable_s2c();
    s2c->set_type(network::ServerToClient::WELCOME);

    server->sendToClient<network::Envelope>(clientId, envelope);
}

void GameManager::handlePrompt()
{
    try
    {
        std::string input;
        std::getline(std::cin, input);
        promptInput.set_value(input);
        promptThreadRunning = false;
    }
    catch (const std::exception &e)
    {
        Logger::error("Error in handlePrompt: {}", e.what());
    }
}
