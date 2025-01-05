#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "HUDBase.h"
#include "EventBus.h"

class GameWorld;

class HUDManager
{
public:
    enum class State
    {
        Menu,
        InGame
    };

    HUDManager(sf::RenderWindow &window, EventBus &eventBus);

    ~HUDManager() = default;
    void setState(State newState);
    void handleEvent(const sf::Event &event);
    void update(float deltaTime);
    void update(GameWorld &world, float deltaTime);
    void render();

private:
    void createMenuHUD();
    void createInGameHUD();

private:
    sf::RenderWindow &window;
    EventBus &eventBus;

    std::unique_ptr<HUDBase> hud;
    State currentState{State::Menu};
};
