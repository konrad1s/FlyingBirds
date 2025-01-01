#pragma once

#include "EntityFactory.h"
#include "ResourceManager.h"

std::unique_ptr<Player> EntityFactory::createPlayer(float x, float y, float mass)
{
    auto &rm = ResourceManager::getInstance();
    auto player = std::make_unique<Player>();

    player->setPosition(x, y);
    player->setMass(mass);

    auto loadTexture = [&](int playerNum, const std::string &directory, int frameCount)
    {
        /* keys like: "p1_flying_1" */
        for (int i = 1; i <= frameCount; i++)
        {
            std::string key = "p" + std::to_string(playerNum) + "_" + directory + "_" + std::to_string(i);
            std::string path = "players/player" + std::to_string(playerNum  + 1) + "/" + directory + "/a" + std::to_string(i) + ".png";

            try
            {
                auto texture = rm.acquire<sf::Texture>(key, path);
                player->getAnimation().addFrame(texture);
            }
            catch (const std::exception &e)
            {
                Logger::error("EntityFactory: Failed to load texture - {}", e.what());
            }
        }
    };

    loadTexture(playerAssetId, "flying", 8);

    playerAssetId = (playerAssetId + 1) % 5;

    return player;
}

std::unique_ptr<Food> EntityFactory::createFood(float x, float y, float mass)
{
    auto &rm = ResourceManager::getInstance();
    auto food = std::make_unique<Food>();

    food->setPosition(x, y);
    food->setMass(mass);

    auto loadTexture = [&](const std::string &directory, int frameCount)
    {
        /* keys like: "p1_flying_1" */
        for (int i = 1; i <= frameCount; i++)
        {
            std::string key = directory + "_" + std::to_string(i);
            std::string path = "items/" + directory + "/a" + std::to_string(i) + ".png";

            try
            {
                auto texture = rm.acquire<sf::Texture>(key, path);
                food->getAnimation().addFrame(texture);
            }
            catch (const std::exception &e)
            {
                Logger::error("EntityFactory: Failed to load texture - {}", e.what());
            }
        }
    };

    loadTexture("food", 8);

    return food;
}
