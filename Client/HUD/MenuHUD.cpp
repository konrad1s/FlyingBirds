#include "MenuHUD.h"
#include <SFML/Graphics.hpp>
#include "Logger.h"
#include "ResourceManager.h"
#include "Events.h"
#include <algorithm>

MenuHUD::MenuHUD(sf::RenderWindow &window, EventBus &bus)
    : focusedInput(FocusedInput::None), eventBus(bus)
{
    initializeHud(window);
}

void MenuHUD::initializeHud(sf::RenderWindow &window)
{
    auto &rm = ResourceManager::getInstance();

    try
    {
        font = rm.acquire<sf::Font>("arial", "fonts/ThaleahFat.ttf");
    }
    catch (...)
    {
        Logger::error("Failed to load font: assets/fonts/ThaleahFat.ttf");
        throw;
    }

    try
    {
        connectTexture = rm.acquire<sf::Texture>("playButton", "hud/play.png");
    }
    catch (...)
    {
        Logger::error("Failed to load texture: assets/hud/play.png");
        throw;
    }

    try
    {
        playInactiveTexture = rm.acquire<sf::Texture>("playInactiveButton", "hud/playInactive.png");
    }
    catch (...)
    {
        Logger::error("Failed to load texture: assets/hud/playInactive.png");
        throw;
    }

    /* Calculate window center */
    sf::Vector2u windowSize = window.getSize();
    float centerX = windowSize.x / 2.f;
    float centerY = windowSize.y / 2.f;

    /* Define vertical offset (positive moves HUD higher) */
    float verticalOffset = 30.f;

    /* Setup Labels */
    auto setupLabel = [&](sf::Text &label, const std::string &text)
    {
        label.setFont(*font);
        label.setString(text);
        label.setCharacterSize(42);
        label.setFillColor(sf::Color::Yellow);
        label.setOutlineColor(sf::Color::Black);
        label.setOutlineThickness(2.f);
    };

    setupLabel(ipLabel, "IP Address:");
    setupLabel(portLabel, "Port:");
    setupLabel(nicknameLabel, "Nickname:");

    float maxLabelWidth = std::max({ ipLabel.getLocalBounds().width, portLabel.getLocalBounds().width, nicknameLabel.getLocalBounds().width });

    /* Setup Connect Button */
    connectButton.setTexture(*connectTexture);
    float desiredButtonWidth = 200.f;
    float scaleFactor = desiredButtonWidth / connectButton.getLocalBounds().width;
    connectButton.setScale(scaleFactor, scaleFactor);
    connectButton.setOrigin(connectButton.getLocalBounds().width / 2.f, connectButton.getLocalBounds().height / 2.f);

    /* Setup Input Boxes */
    auto setupInputBox = [&](sf::RectangleShape &box, const sf::Vector2f &size)
    {
        box.setSize(size);
        box.setFillColor(sf::Color::Black);
        box.setOutlineColor(sf::Color::White);
        box.setOutlineThickness(4.f);
    };

    setupInputBox(ipInputBox, sf::Vector2f(320.f, 40.f));
    setupInputBox(portInputBox, sf::Vector2f(150.f, 40.f));
    setupInputBox(nicknameInputBox, sf::Vector2f(320.f, 40.f));

    /* Space between label-input pairs */
    float verticalSpacing = 75.f;
    float startY = centerY - (2 * verticalSpacing) - verticalOffset;

    /* Position Labels and Input Boxes */
    float labelX = -(maxLabelWidth + 10.f + 300.f) / 2.f;

    /* IP Address Positioning */
    sf::FloatRect ipLabelBounds = ipLabel.getLocalBounds();
    ipLabel.setPosition(labelX, startY - ipLabelBounds.height + 12.f);
    ipInputBox.setPosition(labelX + maxLabelWidth + 20.f, startY);

    /* Port Positioning */
    sf::FloatRect portLabelBounds = portLabel.getLocalBounds();
    portLabel.setPosition(labelX, startY + verticalSpacing - portLabelBounds.height + 12.f);
    portInputBox.setPosition(labelX + maxLabelWidth + 20.f, startY + verticalSpacing);

    /* Nickname Positioning */
    sf::FloatRect nicknameLabelBounds = nicknameLabel.getLocalBounds();
    nicknameLabel.setPosition(labelX, startY + 2 * verticalSpacing - nicknameLabelBounds.height + 12.f);
    nicknameInputBox.setPosition(labelX + maxLabelWidth + 20.f, startY + 2 * verticalSpacing);

    connectButton.setPosition(0.f, startY + 2 * verticalSpacing + 120.f);

    ipLabel.move(centerX, 0.f);
    ipInputBox.move(centerX, 0.f);
    portLabel.move(centerX, 0.f);
    portInputBox.move(centerX, 0.f);
    nicknameLabel.move(centerX, 0.f);
    nicknameInputBox.move(centerX, 0.f);
    connectButton.move(centerX, 0.f);

    /* Setup Status Text */
    statusText.setFont(*font);
    statusText.setCharacterSize(36);
    statusText.setFillColor(sf::Color::White);
    statusText.setOutlineColor(sf::Color::Black);
    statusText.setOutlineThickness(1.5f);
    statusText.setString("STATUS: " + getStatusString());
    sf::FloatRect statusBounds = statusText.getLocalBounds();
    statusText.setOrigin(statusBounds.width / 2.f, statusBounds.height / 2.f);
    statusText.setPosition(centerX, windowSize.y - statusBounds.height - 25.f);

    /* Setup Title Text */
    titleText.setFont(*font);
    titleText.setString("FlyingBirds");
    titleText.setCharacterSize(80);
    titleText.setFillColor(sf::Color::Cyan);
    titleText.setOutlineColor(sf::Color::Black);
    titleText.setOutlineThickness(4.f);

    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(titleBounds.width / 2.f, titleBounds.height / 2.f);
    titleText.setPosition(windowSize.x / 2.f, 50.f);

    updateInputBoxOutlines();
}

void MenuHUD::handleEvent(sf::RenderWindow &window, const sf::Event &event)
{
    if ((currentStatus != Status::Connecting) && (currentStatus != Status::Connected_Waiting))
    {
        switch (event.type)
        {
        case sf::Event::TextEntered:
            handleTextEntered(event);
            break;

        case sf::Event::MouseButtonPressed:
            handleMousePressed(window, event.mouseButton);
            break;

        case sf::Event::KeyPressed:
            handleKeyPressed(event.key);
            break;

        default:
            break;
        }
    }
}

void MenuHUD::handleTextEntered(const sf::Event &event)
{
    if (event.text.unicode == TAB_KEY)
    {
        return;
    }

    if (focusedInput == FocusedInput::IP)
    {
        processInput(ipInput, event.text.unicode, false, 15);
    }
    else if (focusedInput == FocusedInput::Port)
    {
        processInput(portInput, event.text.unicode, true, 5);
    }
    else if (focusedInput == FocusedInput::Nickname)
    {
        processInput(nicknameInput, event.text.unicode, false, 15);
    }
}

void MenuHUD::handleMousePressed(sf::RenderWindow &window, const sf::Event::MouseButtonEvent &mouseEvent)
{
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(mouseEvent.x, mouseEvent.y));
    if (ipInputBox.getGlobalBounds().contains(mousePos))
    {
        focusedInput = FocusedInput::IP;
    }
    else if (portInputBox.getGlobalBounds().contains(mousePos))
    {
        focusedInput = FocusedInput::Port;
    }
    else if (nicknameInputBox.getGlobalBounds().contains(mousePos))
    {
        focusedInput = FocusedInput::Nickname;
    }
    else if (connectButton.getGlobalBounds().contains(mousePos))
    {
        Events::PlayButtonClickEvent event{ getIP(), getPort(), getNickname() };
        eventBus.publish<Events::PlayButtonClickEvent>(event);
    }
    else
    {
        focusedInput = FocusedInput::None;
    }

    updateInputBoxOutlines();
}

void MenuHUD::handleKeyPressed(const sf::Event::KeyEvent &keyEvent)
{
    if (keyEvent.code == sf::Keyboard::Tab)
    {
        switchFocus();
    }
    else if (keyEvent.code == sf::Keyboard::Enter)
    {
        Events::PlayButtonClickEvent event{ getIP(), getPort(), getNickname() };
        eventBus.publish<Events::PlayButtonClickEvent>(event);
    }
}

void MenuHUD::processInput(std::string &input, uint32_t unicode, bool isNumeric, size_t maxLength)
{
    if (unicode == BACKSPACE_KEY)
    {
        if (!input.empty())
        {
            input.pop_back();
        }
    }
    else if (unicode < 128 && input.length() < maxLength)
    {
        if (!isNumeric || std::isdigit(unicode))
        {
            input += static_cast<char>(unicode);
        }
    }
}

void MenuHUD::switchFocus()
{
    if (focusedInput == FocusedInput::IP)
    {
        focusedInput = FocusedInput::Port;
    }
    else if (focusedInput == FocusedInput::Port)
    {
        focusedInput = FocusedInput::Nickname;
    }
    else if (focusedInput == FocusedInput::Nickname)
    {
        focusedInput = FocusedInput::IP;
    }
    else
    {
        focusedInput = FocusedInput::IP;
    }

    updateInputBoxOutlines();
}

void MenuHUD::updateInputBoxOutlines()
{
    if (focusedInput == FocusedInput::IP)
    {
        ipInputBox.setOutlineColor(sf::Color::Yellow);
    }
    else
    {
        ipInputBox.setOutlineColor(sf::Color::White);
    }

    if (focusedInput == FocusedInput::Port)
    {
        portInputBox.setOutlineColor(sf::Color::Yellow);
    }
    else
    {
        portInputBox.setOutlineColor(sf::Color::White);
    }

    if (focusedInput == FocusedInput::Nickname)
    {
        nicknameInputBox.setOutlineColor(sf::Color::Yellow);
    }
    else
    {
        nicknameInputBox.setOutlineColor(sf::Color::White);
    }
}

void MenuHUD::update(float deltaTime)
{
}

void MenuHUD::render(sf::RenderWindow &window)
{
    window.draw(titleText);
    window.draw(ipLabel);
    window.draw(portLabel);
    window.draw(nicknameLabel);
    window.draw(connectButton);
    window.draw(ipInputBox);
    window.draw(portInputBox);
    window.draw(nicknameInputBox);

    auto drawText = [&](const std::string &str, const sf::RectangleShape &box)
    {
        sf::Text text;
        text.setFont(*font);
        text.setString(str);
        text.setCharacterSize(42);
        text.setFillColor(sf::Color::White);
        text.setPosition(box.getPosition().x + 5.f, box.getPosition().y - 12.f);
        window.draw(text);
    };

    drawText(ipInput, ipInputBox);
    drawText(portInput, portInputBox);
    drawText(nicknameInput, nicknameInputBox);

    window.draw(statusText);
}

std::string MenuHUD::getIP() const
{
    return ipInput;
}

unsigned short MenuHUD::getPort() const
{
    try
    {
        return static_cast<unsigned short>(std::stoi(portInput));
    }
    catch (...)
    {
        return 0;
    }
}

std::string MenuHUD::getNickname() const
{
    return nicknameInput;
}

void MenuHUD::setStatus(Status status, sf::RenderWindow &window)
{
    currentStatus = status;
    statusText.setString("STATUS: " + getStatusString());

    switch (currentStatus)
    {
    case Status::Connected_Waiting:
        statusText.setFillColor(sf::Color::Green);
        break;
    case Status::InvalidIPPort:
    case Status::Error:
        statusText.setFillColor(sf::Color::Red);
        break;
    case Status::Connecting:
        statusText.setFillColor(sf::Color::Yellow);
        break;
    case Status::Disconnected:
    default:
        statusText.setFillColor(sf::Color::White);
        break;
    }

    sf::FloatRect statusBounds = statusText.getLocalBounds();
    statusText.setOrigin(statusBounds.width / 2.f, statusBounds.height / 2.f);

    sf::Vector2u windowSize = window.getSize();
    statusText.setPosition(windowSize.x / 2.f, windowSize.y - statusBounds.height - 25.f);

    if (status == Status::Connected_Waiting)
    {
        connectButton.setTexture(*playInactiveTexture);
    }
    else
    {
        connectButton.setTexture(*connectTexture);
    }
}

std::string MenuHUD::getStatusString() const
{
    switch (currentStatus)
    {
    case Status::Disconnected:
        return "Disconnected";
    case Status::Connecting:
        return "Connecting...";
    case Status::Connected_Waiting:
        return "Connected - waiting for the game to be started";
    case Status::InvalidIPPort:
        return "Invalid IP or Port";
    case Status::Error:
        return "Error occurred";
    default:
        return "Unknown Status";
    }
}
