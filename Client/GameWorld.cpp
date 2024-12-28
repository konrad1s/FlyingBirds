#include "GameWorld.h"

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
    for (auto &p : message.players())
    {
        auto it = players.find(p.id());

        if (it == players.end())
        {
            /* New player detected */
            auto newPlayer = std::make_unique<Player>();

            auto dummyTexture = std::make_unique<sf::Texture>();
            dummyTexture->create(50, 50);

            newPlayer->setTexture(dummyTexture.get());
            newPlayer->setPosition(p.position().x(), p.position().y());

            players.emplace(p.id(), std::move(newPlayer));
        }
        else
        {
            it->second->setPosition(p.position().x(), p.position().y());
            it->second->setHealth(p.size());
        }
    }

    for (auto &f : message.foods())
    {
        auto it = foods.find(f.id());

        if (it == foods.end())
        {
            auto newFood = std::make_unique<Food>();

            auto dummyTexture = std::make_unique<sf::Texture>();
            dummyTexture->create(20, 20);

            newFood->setTexture(dummyTexture.get());
            newFood->setPosition(f.position().x(), f.position().y());

            foods.emplace(f.id(), std::move(newFood));
        }
        else
        {
            it->second->setPosition(f.position().x(), f.position().y());
        }
    }
}
