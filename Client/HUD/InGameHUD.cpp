#include "InGameHUD.h"
#include "ResourceManager.h"
#include "Logger.h"
#include "GameWorld.h"
#include "Player.h"
#include <algorithm>

InGameHUD::InGameHUD(sf::RenderWindow &window)
{
    initializeHud(window);
}

void InGameHUD::initializeHud(sf::RenderWindow &window)
{
    auto &resourceManager = ResourceManager::getInstance();

    try
    {
        font = resourceManager.acquire<sf::Font>("arial", "fonts/arial.ttf");
    }
    catch (const std::exception &e)
    {
        Logger::error("Failed to load font for InGameHUD: {}", e.what());
    }

    try
    {
        separatorTexture = resourceManager.acquire<sf::Texture>("separator", "hud/separator.png");
    }
    catch (const std::exception &e)
    {
        Logger::error("Failed to load separator texture for InGameHUD: {}", e.what());
    }

    try
    {
        boardTexture = resourceManager.acquire<sf::Texture>("board", "hud/board.png");
    }
    catch (const std::exception &e)
    {
        Logger::error("Failed to load board texture for InGameHUD: {}", e.what());
    }

    try
    {
        protectionTexture = resourceManager.acquire<sf::Texture>("protectionIcon", "items/protection.png");
    }
    catch (const std::exception &e)
    {
        Logger::error("Failed to load protection icon texture for InGameHUD: {}", e.what());
    }

    try
    {
        speedBoostTexture = resourceManager.acquire<sf::Texture>("speedBoostIcon", "items/power-up.png");
    }
    catch (const std::exception &e)
    {
        Logger::error("Failed to load speed boost icon texture for InGameHUD: {}", e.what());
    }

    setupSeparator(window);
    setupTitleText();
}

void InGameHUD::setupSeparator(sf::RenderWindow &window)
{
    if (!separatorTexture)
        return;

    separatorSprite.setTexture(*separatorTexture);
    separatorSprite.setPosition(0.f, 0.f);

    const float windowHeight = static_cast<float>(window.getSize().y);
    const float spriteHeight = separatorSprite.getLocalBounds().height;

    if (spriteHeight > 0.f)
    {
        const float scaleY = windowHeight / spriteHeight;
        separatorSprite.setScale(1.f, scaleY);
    }
}

void InGameHUD::setupTitleText()
{
    if (!font)
        return;

    titleText.setFont(*font);
    titleText.setString("Players");
    titleText.setCharacterSize(42);
    titleText.setFillColor(sf::Color::Black);
    titleText.setPosition(START_X, 10.f);

    timeText.setFont(*font);
    timeText.setCharacterSize(36);
    timeText.setFillColor(sf::Color::Red);
    timeText.setPosition(START_X + 200.0, 15.f);
}

void InGameHUD::handleEvent(sf::RenderWindow &, const sf::Event &)
{
}

void InGameHUD::update(GameWorld &world, float deltaTime)
{
    const auto &activePlayers = world.getPlayers();

    std::size_t idx = 0;
    for (const auto &[playerId, playerPtr] : activePlayers)
    {
        auto [it, inserted] = playerInfos.try_emplace(playerId, PlayerInfo{});
        auto &info = it->second;

        if (inserted)
        {
            info = createPlayerInfo(playerPtr.get(), idx);
        }
        else
        {
            updatePlayerInfo(info, playerPtr.get(), idx);
        }

        ++idx;
    }
}

void InGameHUD::render(sf::RenderWindow &window)
{
    window.draw(separatorSprite);
    window.draw(titleText);

    int timeInt = static_cast<int>(std::ceil(timeRemaining));
    timeText.setString("Remaining time: " + std::to_string(timeInt));
    window.draw(timeText);

    for (auto &kv : playerInfos)
    {
        auto &info = kv.second;
        window.draw(info.boardSprite);
        window.draw(info.nicknameText);
        window.draw(info.scoreText);
        window.draw(info.playerSkinSprite);

        if (info.protectionIcon.getPosition().x >= 0.f)
        {
            window.draw(info.protectionIcon);
        }
        if (info.speedBoostIcon.getPosition().x >= 0.f)
        {
            window.draw(info.speedBoostIcon);
        }
    }
}

InGameHUD::PlayerInfo InGameHUD::createPlayerInfo(const Player *playerPtr, std::size_t index) const
{
    PlayerInfo info;

    if (boardTexture)
    {
        info.boardSprite.setTexture(*boardTexture);
        constexpr float boardWidth = 490.f;
        constexpr float boardHeight = 90.f;

        const auto bounds = info.boardSprite.getLocalBounds();
        if (bounds.width > 0.f && bounds.height > 0.f)
        {
            const float scaleX = boardWidth / bounds.width;
            const float scaleY = boardHeight / bounds.height;
            info.boardSprite.setScale(scaleX, scaleY);
        }

        info.boardSprite.setColor(sf::Color(255, 255, 255, 150));
        info.boardSprite.setPosition(60.f, 63.f + VERTICAL_SPACING * static_cast<float>(index));
    }

    if (font)
    {
        info.nicknameText.setFont(*font);
        info.nicknameText.setString(playerPtr->getNickname());
        info.nicknameText.setCharacterSize(28);
        info.nicknameText.setFillColor(sf::Color::Black);
        info.nicknameText.setPosition(START_X, 70.f + VERTICAL_SPACING * static_cast<float>(index));
    }

    if (font)
    {
        info.scoreText.setFont(*font);
        info.scoreText.setString("Score: " + std::to_string(static_cast<int>(playerPtr->getMass())));
        info.scoreText.setCharacterSize(28);
        info.scoreText.setFillColor(sf::Color::Black);
        info.scoreText.setPosition(START_X, 100.f + VERTICAL_SPACING * static_cast<float>(index));
    }

    if (auto playerTexture = playerPtr->getTexture())
    {
        info.playerSkinSprite.setTexture(*playerTexture);
        constexpr float desiredSize = 64.f;
        const float scale = desiredSize / static_cast<float>(std::max(playerTexture->getSize().x,
                                                                      playerTexture->getSize().y));
        info.playerSkinSprite.setScale(scale, scale);
        info.playerSkinSprite.setPosition(START_X + 440.f - desiredSize,
                                          80.f + VERTICAL_SPACING * static_cast<float>(index));
    }

    if (protectionTexture)
    {
        info.protectionIcon.setTexture(*protectionTexture);
        constexpr float desiredSize = 24.f;
        const float scale = desiredSize / static_cast<float>(std::max(protectionTexture->getSize().x,
                                                                      protectionTexture->getSize().y));
        info.protectionIcon.setScale(scale, scale);
        info.protectionIcon.setPosition(-100.f, -100.f);
    }

    if (speedBoostTexture)
    {
        info.speedBoostIcon.setTexture(*speedBoostTexture);
        constexpr float desiredSize = 24.f;
        const float scale = desiredSize / static_cast<float>(std::max(speedBoostTexture->getSize().x,
                                                                      speedBoostTexture->getSize().y));
        info.speedBoostIcon.setScale(scale, scale);
        info.speedBoostIcon.setPosition(-100.f, -100.f);
    }

    return info;
}

void InGameHUD::updatePlayerInfo(PlayerInfo &info, const Player *playerPtr, std::size_t index)
{
    info.scoreText.setString("Score: " + std::to_string(static_cast<int>(playerPtr->getMass())));

    if (auto playerTexture = playerPtr->getTexture())
    {
        if (info.playerSkinSprite.getTexture() != playerTexture.get())
        {
            info.playerSkinSprite.setTexture(*playerTexture);
            constexpr float desiredSize = 64.f;
            const float scale = desiredSize / static_cast<float>(std::max(playerTexture->getSize().x,
                                                                          playerTexture->getSize().y));
            info.playerSkinSprite.setScale(scale, scale);
            info.playerSkinSprite.setPosition(START_X + 440.f - desiredSize,
                                              80.f + VERTICAL_SPACING * static_cast<float>(index));
        }
    }

    if (info.protectionIcon.getTexture())
    {
        if (playerPtr->isProtectionActive())
        {
            info.protectionIcon.setPosition(START_X + 200.f, 110.f + VERTICAL_SPACING * static_cast<float>(index));
        }
        else
        {
            info.protectionIcon.setPosition(-100.f, -100.f);
        }
    }

    if (info.speedBoostIcon.getTexture())
    {
        if (playerPtr->isSpeedBoostActive())
        {
            info.speedBoostIcon.setPosition(START_X + 230.f, 110.f + VERTICAL_SPACING * static_cast<float>(index));
        }
        else
        {
            info.speedBoostIcon.setPosition(-100.f, -100.f);
        }
    }
}

void InGameHUD::setTimeRemaining(float time)
{
    timeRemaining = time;
}
