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
    gameWorld.addPlayer(clientId);
    Logger::info("Player {} connected to the game.", clientId);
    sendWelcomeToClient(clientId);
}

void GameManager::onClientDisconnected(uint32_t clientId)
{
    gameWorld.removePlayer(clientId);
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

    auto player = s2c->add_entities();
    player->set_id(clientId);
    player->set_entitytype(network::ServerToClient::Entity::PLAYER);

    server->sendToClient<network::Envelope>(clientId, envelope);
}

void GameManager::broadcastGameState()
{
    using namespace network;

    Envelope envelope;
    envelope.set_category(Envelope::SERVER_TO_CLIENT);
    auto s2c = envelope.mutable_s2c();
    s2c->set_type(ServerToClient::STATE_UPDATE);

    for (auto &kv : gameWorld.getPlayers())
    {
        Player *playerPtr = kv.second.get();
        auto netEnt = s2c->add_entities();

        netEnt->set_id(playerPtr->getId());
        netEnt->mutable_position()->set_x(playerPtr->getX());
        netEnt->mutable_position()->set_y(playerPtr->getY());
        netEnt->set_mass(playerPtr->getMass());
        netEnt->set_entitytype(ServerToClient::Entity::PLAYER);
    }

    for (auto &entityPtr : gameWorld.getEntities())
    {
        auto netEnt = s2c->add_entities();

        netEnt->set_id(entityPtr->getId());
        netEnt->mutable_position()->set_x(entityPtr->getX());
        netEnt->mutable_position()->set_y(entityPtr->getY());
        netEnt->set_mass(entityPtr->getMass());

        switch (entityPtr->getEntityType())
        {
        case EntityType::Food:
            netEnt->set_entitytype(ServerToClient::Entity::FOOD);
            break;
        case EntityType::SpeedBoost:
            netEnt->set_entitytype(ServerToClient::Entity::SPEEDBOOST);
            break;
        default:
            netEnt->set_entitytype(ServerToClient::Entity::UNKNOWN);
            break;
        }
    }

    server->broadcast(envelope);
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
