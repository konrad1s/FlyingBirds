#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "HUDBase.h"
#include "EventBus.h"
#include "MenuHUD.h"
#include "GameEndHUD.h"

class GameWorld;

class HUDManager
{
public:
    enum class State
    {
        Menu,
        InGame,
        GameEnd
    };

    HUDManager(sf::RenderWindow &window, EventBus &eventBus);

    ~HUDManager() = default;
    void setState(State newState);
    std::unique_ptr<HUDBase>& getHUD() { return hud; }
    void handleEvent(const sf::Event &event);
    void update(float deltaTime);
    void update(GameWorld &world, float deltaTime);
    void render();

    void setMenuStatus(MenuHUD::Status status);

private:
    void createMenuHUD();
    void createInGameHUD();
    void createGameEndHUD();

private:
    sf::RenderWindow &window;
    EventBus &eventBus;

    std::unique_ptr<HUDBase> hud;
    State currentState{State::Menu};
};
