#pragma once
// #include "EntityManager.h"
#include <SFML/Graphics.hpp>
#include "network.pb.h"

class GameWorld {
public:
    GameWorld() = default;

    void update(float dt);
    void render(sf::RenderWindow &window);

    // void handleGameStateUpdate();

    // EntityManager& getEntityManager() { return em; }

private:
    // EntityManager em;
};
