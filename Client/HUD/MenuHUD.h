#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "EventBus.h"

class MenuHUD
{
public:
    MenuHUD(sf::RenderWindow &window, EventBus &eventBus);
    void handleEvent(sf::RenderWindow &window, const sf::Event &event);
    void update(float deltaTime);
    void render(sf::RenderWindow &window);

private:
    void initializeHud(sf::RenderWindow &window);
    void handleTextEntered(const sf::Event &event);
    void handleMousePressed(sf::RenderWindow &window, const sf::Event::MouseButtonEvent &mouseEvent);
    void handleKeyPressed(const sf::Event::KeyEvent &keyEvent);
    void processInput(std::string &input, uint32_t unicode, bool isNumeric, size_t maxLength);
    void switchFocus();
    void updateInputBoxOutlines();
    std::string getIP() const;
    unsigned short getPort() const;
    std::string getNickname() const;

    enum class FocusedInput
    {
        None,
        IP,
        Port,
        Nickname
    } focusedInput;

    const uint32_t TAB_KEY = 9;
    const uint32_t BACKSPACE_KEY = 8;
    const uint32_t ENTER_KEY = 13;
    EventBus &eventBus;

    std::shared_ptr<sf::Font> font;
    std::shared_ptr<sf::Texture> connectTexture;

    sf::Text ipLabel;
    sf::Text portLabel;
    sf::Text nicknameLabel;
    sf::Sprite connectButton;
    sf::RectangleShape ipInputBox;
    sf::RectangleShape portInputBox;
    sf::RectangleShape nicknameInputBox;
    std::string ipInput;
    std::string portInput;
    std::string nicknameInput;
};
