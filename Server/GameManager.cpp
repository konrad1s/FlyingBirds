#include <iostream>
#include "GameManager.h"
#include "Logger.h"
#include "Events.h"

GameManager::GameManager()
    : state(State::waitingForClients), scoreboardManager(eventBus), gameWorld(eventBus)
{
    promptFuture = promptInput.get_future();
    server = std::make_unique<Server>(eventBus);

    eventBus.subscribe<Events::ClientConnectedEvent>(
        [this](const Events::ClientConnectedEvent &e)
        {
            this->onClientConnected(e.clientId, e.nickname);
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

        if (gameWorld.getPlayers().size() >= 5)
        {
            state = State::starting;
            Logger::info("Starting the game with {} players.", gameWorld.getPlayers().size());
        }
    }
    else if (state == State::starting)
    {
        if (promptThread.joinable())
            promptThread.join();
        promptThreadRunning = false;

        timeRemaining = 60.f;
        isGameOver = false;
        scoreboardManager.reset();

        broadcastGameStart();
        state = State::running;
        Logger::info("GameManager state changed to running.");
    }
    else if (state == State::running)
    {
        timeRemaining -= deltaTime;
        gameWorld.update(deltaTime);
        broadcastGameState();
        handleEndGame(deltaTime);
    }
    else if (state == State::finished)
    {
        isGameOver = true;
        broadcastScoreboard();
        promptThreadRunning = true;
        promptThread = std::thread(&GameManager::handlePrompt, this);
        state = State::waitingForClients;
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
                Logger::info("GameManager starting via console command.");
            }
            else if (!input.empty())
            {
                Logger::warning("Unknown command received: {}", input);
            }

            {
                std::lock_guard<std::mutex> lock(promptMutex);
                promptInput = std::promise<std::string>();
                promptFuture = promptInput.get_future();
            }

            if (!promptThreadRunning && state == State::waitingForClients)
            {
                if (promptThread.joinable())
                {
                    promptThread.join();
                }
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

void GameManager::onClientConnected(uint32_t clientId, const std::string &nickname)
{
    if (!nickname.empty())
    {
        gameWorld.addPlayer(clientId, nickname);
        Logger::info("Player {} connected to the game.", clientId);
        sendWelcomeToClient(clientId);
    }
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
    s2c->set_time_remaining(timeRemaining);

    for (auto& [id, playerPtr] : gameWorld.getPlayers())
    {
        ServerToClient::Entity* netEnt = s2c->add_entities();
        netEnt->set_id(playerPtr->getId());
        netEnt->mutable_position()->set_x(playerPtr->getX());
        netEnt->mutable_position()->set_y(playerPtr->getY());
        
        netEnt->set_speedboostactive(playerPtr->isSpeedBoostActive());
        netEnt->set_protectionactive(playerPtr->isShieldActive());
        netEnt->set_mass(playerPtr->getMass());
        netEnt->set_attacking(playerPtr->isAttacking());

        netEnt->set_entitytype(ServerToClient::Entity::PLAYER);
        netEnt->set_nickname(playerPtr->getNickname());
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
        case EntityType::Protection:
            netEnt->set_entitytype(ServerToClient::Entity::PROTECTION);
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

void GameManager::broadcastGameStart()
{
    using namespace network;

    Envelope envelope;
    envelope.set_category(Envelope::SERVER_TO_CLIENT);

    ServerToClient* s2c = envelope.mutable_s2c();
    s2c->set_type(ServerToClient::GAME_START);

    server->broadcast(envelope);

    Logger::info("Broadcasted GAME_START message to all clients.");
}

void GameManager::broadcastScoreboard()
{
    using namespace network;

    std::vector<Player*> livingPlayers;
    livingPlayers.reserve(gameWorld.getPlayers().size());

    for (auto &kv : gameWorld.getPlayers())
    {
        Player* p = kv.second.get();
        if (p && p->getMass() > 0.f)
        {
            livingPlayers.push_back(p);
        }
    }

    std::sort(livingPlayers.begin(), livingPlayers.end(), 
              [](Player* a, Player* b)
              {
                  return a->getMass() > b->getMass();
              });

    const auto &deadPlayersOrder = scoreboardManager.getDeadPlayersOrder();

    Envelope envelope;
    envelope.set_category(Envelope::SERVER_TO_CLIENT);

    auto s2c = envelope.mutable_s2c();
    s2c->set_type(ServerToClient::GOODBYE); 

    for (size_t i = 0; i < livingPlayers.size(); ++i)
    {
        ServerToClient::Entity* netEnt = s2c->add_entities();
        netEnt->set_id(livingPlayers[i]->getId());
        netEnt->set_entitytype(ServerToClient::Entity::PLAYER);
        netEnt->set_mass(livingPlayers[i]->getMass());

        Logger::info("  Rank {}: ID={}", i + 1, livingPlayers[i]->getId());
    }

    if (!deadPlayersOrder.empty())
    {
        for (size_t i = deadPlayersOrder.size(); i-- > 0; )
        {
            uint32_t deadId = deadPlayersOrder[i];
            Player* deadPlayer = gameWorld.findPlayerById(deadId);

            if (deadPlayer)
            {
                ServerToClient::Entity* netEnt = s2c->add_entities();
                netEnt->set_id(deadId);
                netEnt->set_entitytype(ServerToClient::Entity::PLAYER);
                netEnt->set_mass(0.f);

                Logger::info("  Death Order {}: ID={}", deadPlayersOrder.size() - i, deadId);
            }
            else
            {
                Logger::warning("Dead player with ID={} not found in gameWorld.", deadId);
            }
        }
    }
    else
    {
        Logger::info("No dead players to display in the scoreboard.");
    }

    server->broadcast(envelope);
    Logger::info("Broadcasted final scoreboard.");
}

void GameManager::handleEndGame(float deltaTime)
{
    if (timeRemaining <= 0.0f && !isGameOver)
    {
        Logger::info("Time is up! Game over.");
        state = State::finished;
        return;
    }

    int aliveCount = 0;
    for (auto &kv : gameWorld.getPlayers())
    {
        Player *p = kv.second.get();
        if (p && p->getMass() > 0.f)
            aliveCount++;
    }

    if (aliveCount <= 1 && !isGameOver)
    {
        Logger::info("Only {} player(s) remain(s). Game over!", aliveCount);
        state = State::finished;
        return;
    }
}
