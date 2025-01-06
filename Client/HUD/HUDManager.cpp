#include "HUDManager.h"
#include "MenuHUD.h"
#include "InGameHUD.h"
#include "GameWorld.h"

HUDManager::HUDManager(sf::RenderWindow &win, EventBus &bus): window(win), eventBus(bus)
{
    createMenuHUD();
}

void HUDManager::setState(State newState)
{
    if (newState == currentState)
        return;

    switch (newState)
    {
    case State::Menu:
        createMenuHUD();
        break;
    case State::InGame:
        createInGameHUD();
        break;
    }

    currentState = newState;
}

void HUDManager::handleEvent(const sf::Event &event)
{
    if (hud)
    {
        hud->handleEvent(window, event);
    }
}

void HUDManager::update(float deltaTime)
{
    if (hud)
    {
        hud->update(deltaTime);
    }
}

void HUDManager::update(GameWorld &world, float deltaTime)
{
    if (hud && currentState == State::InGame)
    {
        if (auto inGameHUD = dynamic_cast<InGameHUD *>(hud.get()))
        {
            inGameHUD->update(world, deltaTime);
        }
    }
}

void HUDManager::render()
{
    if (hud)
    {
        hud->render(window);
    }
}

void HUDManager::createMenuHUD()
{
    hud = std::make_unique<MenuHUD>(window, eventBus);
}

void HUDManager::createInGameHUD()
{
    hud = std::make_unique<InGameHUD>(window);
}

void HUDManager::setMenuStatus(MenuHUD::Status status)
{
    if (currentState == State::Menu)
    {
        if (auto menuHud = dynamic_cast<MenuHUD*>(hud.get()))
        {
            menuHud->setStatus(status, window);
        }
    }
}
