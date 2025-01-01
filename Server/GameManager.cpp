#include <iostream>
#include "GameManager.h"
#include "Logger.h"
#include "Events.h"

GameManager::GameManager()
    : state(State::waitingForClients)
{
    promptFuture = promptInput.get_future();
    server = std::make_unique<Server>(eventBus);

    eventBus.subscribe<Events::ClientConnectedEvent>(
        [this](const Events::ClientConnectedEvent &e)
        {
            this->onClientConnected(e.clientId);
        });

    eventBus.subscribe<Events::ClientDataUpdatedEvent>(
        [this](const Events::ClientDataUpdatedEvent &e)
        {
            this->onClientMessage(e.clientId, e.message);
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

        server->update();
        update(deltaTime);

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

        gameWorld.spawnFood();

        state = State::running;
        Logger::info("GameManager state changed to running.");
    }
    else if (state == State::running)
    {
        gameWorld.update(deltaTime);
        broadcastGameState();
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
    gameWorld.addPlayer(clientId);
    Logger::info("Player {} connected to the game.", clientId);

    sendWelcomeToClient(clientId);
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
    float angleDegrees = msg.angle();
    gameWorld.updatePlayerAngle(clientId, angleDegrees);
}

void GameManager::sendWelcomeToClient(uint32_t clientId)
{
    network::Envelope envelope;
    envelope.set_category(network::Envelope::SERVER_TO_CLIENT);

    network::ServerToClient *s2c = envelope.mutable_s2c();
    s2c->set_type(network::ServerToClient::WELCOME);

    auto player = s2c->add_players();
    player->set_id(clientId);

    server->sendToClient<network::Envelope>(clientId, envelope);
}

void GameManager::broadcastGameState()
{
    network::Envelope envelope;
    envelope.set_category(network::Envelope::SERVER_TO_CLIENT);

    network::ServerToClient* s2c = envelope.mutable_s2c();
    s2c->set_type(network::ServerToClient::STATE_UPDATE);

    for (auto& [id, playerPtr] : gameWorld.getPlayers())
    {
        network::ServerToClient::Entity *entityPlayer = s2c->add_players();
    
        entityPlayer->set_id(id);
        entityPlayer->mutable_position()->set_x(playerPtr->getX());
        entityPlayer->mutable_position()->set_y(playerPtr->getY());
        entityPlayer->set_mass(playerPtr->getMass());
    }

    for (auto &food : gameWorld.getFood())
    {
        network::ServerToClient::Entity *entityFood = s2c->add_foods();

        entityFood->set_id(food.getId());
        entityFood->mutable_position()->set_x(food.getX());
        entityFood->mutable_position()->set_y(food.getY());
        entityFood->set_mass(food.getRadius());
    }

    server->broadcast<network::Envelope>(envelope);
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
