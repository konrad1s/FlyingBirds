#pragma once
// #include "EntityManager.hpp"
#include <SFML/Graphics.hpp>
#include "network.pb.h"

class GameWorld {
public:
    GameWorld() = default;

    void update(float dt);
    void render(sf::RenderWindow &window);

    // void handleGameStateUpdate(const agario::GameState &state);

    // EntityManager& getEntityManager() { return em; }

private:
    // EntityManager em;
};
