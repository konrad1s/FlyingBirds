#include "GameWorld.h"
#include "ResourceManager.h"
#include <unordered_set>

GameWorld::GameWorld(int windowSizeX, int windowSizeY)
{
    try
    {
        auto &resMgr = ResourceManager::getInstance();
        backgroundTexture = resMgr.acquire<sf::Texture>("background", "backgrounds/background.png");

        background.setTexture(*backgroundTexture);
        background.setPosition(0.f, 0.f);

        const auto textureSize = backgroundTexture->getSize();

        float scaleX = static_cast<float>(windowSizeX) / static_cast<float>(textureSize.x);
        float scaleY = static_cast<float>(windowSizeY) / static_cast<float>(textureSize.y);

        float scale = std::min(scaleX, scaleY);
        background.setScale(scale, scale);

        float posX = (windowSizeX - textureSize.x * scale) / 2.f;
        float posY = (windowSizeY - textureSize.y * scale) / 2.f;
        background.setPosition(posX, posY);
    }
    catch (const std::exception &e)
    {
        Logger::error("Failed to load background: {}", e.what());
    }
}

void GameWorld::update(float dt)
{
    for (auto &[id, p] : players)
    {
        p->update(dt);
    }
    for (auto &[id, f] : foods)
    {
        f->update(dt);
    }
}

void GameWorld::render(sf::RenderWindow &window)
{
    window.draw(background);

    for (auto &[id, p] : players)
    {
        p->render(window);
    }

    for (auto &[id, f] : foods)
    {
        f->render(window);
    }
}

void GameWorld::updateFromServer(const network::ServerToClient &message)
{
    std::unordered_set<uint32_t> updatedPlayerIds;
    std::unordered_set<uint32_t> updatedFoodIds;
    auto &res = ResourceManager::getInstance();

    for (const auto &p : message.players())
    {
        updatedPlayerIds.insert(p.id());
        auto it = players.find(p.id());
        if (it == players.end())
        {
            auto newPlayer = factory.createPlayer(p.position().x(),
                                                  p.position().y(),
                                                  p.mass());
            players.emplace(p.id(), std::move(newPlayer));
        }
        else
        {
            it->second->setPosition(p.position().x(), p.position().y());
            it->second->setMass(p.mass());
        }
    }

    for (auto it = players.begin(); it != players.end(); )
    {
        if (updatedPlayerIds.find(it->first) == updatedPlayerIds.end())
        {
            it = players.erase(it);
        }
        else
        {
            ++it;
        }
    }

    for (const auto &f : message.foods())
    {
        updatedFoodIds.insert(f.id());
        auto it = foods.find(f.id());
        if (it == foods.end())
        {
            auto newFood = factory.createFood(f.position().x(),
                                              f.position().y(),
                                              f.mass());

            foods.emplace(f.id(), std::move(newFood));
        }
        else
        {
            it->second->setPosition(f.position().x(), f.position().y());
            it->second->setMass(f.mass());
        }
    }

    for (auto it = foods.begin(); it != foods.end(); )
    {
        if (updatedFoodIds.find(it->first) == updatedFoodIds.end())
        {
            it = foods.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
