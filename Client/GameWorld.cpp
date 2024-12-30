#include "GameWorld.h"
#include <unordered_set>

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

    for (const auto &p : message.players())
    {
        updatedPlayerIds.insert(p.id());
        auto it = players.find(p.id());
        if (it == players.end())
        {
            auto newPlayer = std::make_unique<Player>();

            auto dummyTexture = std::make_unique<sf::Texture>();
            dummyTexture->create(20, 20);
            newPlayer->setTexture(dummyTexture.get());

            newPlayer->setPosition(p.position().x(), p.position().y());
            newPlayer->setMass(p.mass());

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
            auto newFood = std::make_unique<Food>();

            auto dummyTexture = std::make_unique<sf::Texture>();
            dummyTexture->create(50, 50);  
            newFood->setTexture(dummyTexture.get());

            newFood->setPosition(f.position().x(), f.position().y());
            newFood->setMass(f.mass());

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
