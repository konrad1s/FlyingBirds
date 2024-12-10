#pragma once

#include "GameManager.h"
#include <iostream>

GameManager::GameManager() : state(State::waitingForClients)
{
    promptFuture = promptInput.get_future();
}

GameManager::~GameManager()
{
    if (promptThread.joinable())
        promptThread.join();
}

void GameManager::checkPrompt()
{
    if (promptFuture.wait_for(static_cast<std::chrono::microseconds>(0)) == std::future_status::ready)
    {
        try
        {
            std::string input = promptFuture.get();
            if (input == "START")
                state = State::starting;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error handling prompt input: " << e.what() << std::endl;
        }
    }
}

void GameManager::update(float deltaTime)
{
    switch (state)
    {
    case State::waitingForClients:
        if (!promptThreadRunning)
        {
            promptThreadRunning = true;
            promptThread = std::thread(&GameManager::handlePrompt, this);
        }
        else
        {
            checkPrompt();
        }
        break;
    case State::starting:
        promptThread.join();
        promptThreadRunning = false;
        state = State::running;
        break;
    case State::running:
        /* TODO: Handle runinning state*/
        break;
    case State::finished:
        /* TODO: Handle finished state*/
        break;
    }
}

GameManager::State GameManager::getState()
{
    return state;
}

void GameManager::onClientConnected(uint32_t clientId)
{
    clients[clientId] = std::make_unique<Player>(clientId);
}

void GameManager::onClientDisconnected(uint32_t clientId)
{
    clients[clientId].reset();
}

void GameManager::handleClientMessage(uint32_t clientId)
{

}

void GameManager::serializeGameData()
{

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
        std::cerr << "Error in handlePrompt: " << e.what() << std::endl;
    }
}