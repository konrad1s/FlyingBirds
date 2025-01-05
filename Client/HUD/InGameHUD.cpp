#include "InGameHUD.h"
#include "ResourceManager.h"
#include "Logger.h"
#include "GameWorld.h"
#include "Player.h"

InGameHUD::InGameHUD(sf::RenderWindow &window)
{
    initializeHud(window);
}

void InGameHUD::initializeHud(sf::RenderWindow &window)
{
    try
    {
        font = ResourceManager::getInstance().acquire<sf::Font>("arial", "fonts/arial.ttf");
    }
    catch (const std::exception &e)
    {
        Logger::error("Failed to load font for InGameHUD: {}", e.what());
    }

    try
    {
        separatorTexture = ResourceManager::getInstance().acquire<sf::Texture>("separator", "hud/separator.png");
    }
    catch (const std::exception &e)
    {
        Logger::error("Failed to load separator texture for InGameHUD: {}", e.what());
    }

    try
    {
        boardTexture = ResourceManager::getInstance().acquire<sf::Texture>("board", "hud/board.png");
    }
    catch (const std::exception &e)
    {
        Logger::error("Failed to load board texture for InGameHUD: {}", e.what());
    }

    setupSeparator(window);
    setupTitleText();
}

void InGameHUD::setupSeparator(sf::RenderWindow& window)
{
    separatorSprite.setTexture(*separatorTexture);
    separatorSprite.setPosition(0.f, 0.f);

    float windowHeight = static_cast<float>(window.getSize().y);
    float spriteHeight = separatorSprite.getLocalBounds().height;
    float scaleY = windowHeight / spriteHeight;
    separatorSprite.setScale(1.f, scaleY);
}

void InGameHUD::setupTitleText()
{
    titleText.setFont(*font);
    titleText.setString("Players");
    titleText.setCharacterSize(42);
    titleText.setFillColor(sf::Color::Black);
    titleText.setPosition(START_X, 10.f);
}

void InGameHUD::handleEvent(sf::RenderWindow &window, const sf::Event &event)
{
}

void InGameHUD::update(GameWorld &world, float deltaTime)
{
    const auto& activePlayers = world.getPlayers();

    for (const auto &[playerId, playerPtr] : activePlayers)
    {
        if (playerInfos.find(playerId) == playerInfos.end())
        {
            PlayerInfo info;

            // Setup Board Sprite
            info.boardSprite.setTexture(*boardTexture);
            float boardWidth = 490.f;
            float boardHeight = 90.f;
            float scaleX = boardWidth / info.boardSprite.getLocalBounds().width;
            float scaleY = boardHeight / info.boardSprite.getLocalBounds().height;
            info.boardSprite.setScale(scaleX, scaleY);
            info.boardSprite.setColor(sf::Color(255, 255, 255, 150));
            info.boardSprite.setPosition(60.f, 63.f + VERTICAL_SPACING * playerInfos.size());

            // Setup Nickname Text
            info.nicknameText.setFont(*font);
            info.nicknameText.setString("Konrad"); /* Currently not used */
            info.nicknameText.setCharacterSize(28);
            info.nicknameText.setFillColor(sf::Color::Black);
            info.nicknameText.setPosition(START_X, 70.f + VERTICAL_SPACING * playerInfos.size());

            // Setup Score Text
            info.scoreText.setFont(*font);
            info.scoreText.setString("Score: " + std::to_string(static_cast<int>(playerPtr->getMass())));
            info.scoreText.setCharacterSize(28);
            info.scoreText.setFillColor(sf::Color::Black);
            info.scoreText.setPosition(START_X, 100.f + VERTICAL_SPACING * playerInfos.size());

            // Setup Health Text
            info.healthText.setFont(*font);
            info.healthText.setString("Health: 65"); /* Currently not used */
            info.healthText.setCharacterSize(28);
            info.healthText.setFillColor(sf::Color::Black);
            info.healthText.setPosition(START_X + 200.f, 100.f + VERTICAL_SPACING * playerInfos.size());

            // Setup Player Skin Sprite
            std::shared_ptr<sf::Texture> playerTexture = playerPtr->getTexture();
            if (playerTexture)
            {
                info.playerSkinSprite.setTexture(*playerTexture);
                float desiredSize = 64.f;
                float scale = desiredSize / std::max(playerTexture->getSize().x, playerTexture->getSize().y);
                info.playerSkinSprite.setScale(scale, scale);
                info.playerSkinSprite.setPosition(START_X + boardWidth - desiredSize - 50.f, 80.f + VERTICAL_SPACING * playerInfos.size());
            }

            playerInfos.emplace(playerId, info);
        }
        else
        {
            // Update existing player info
            auto &info = playerInfos[playerId];
            Player* playerPtrNonConst = const_cast<Player*>(playerPtr.get());

            // Update Nickname, Score, and Health
            info.nicknameText.setString("Konrad"); /* currently not used */
            info.scoreText.setString("Score: " + std::to_string(static_cast<int>(playerPtr->getMass())));
            // info.healthText.setString("Health: " + std::to_string(playerPtr->getHealth()));

            std::shared_ptr<sf::Texture> playerTexture = playerPtr->getTexture();
            if (playerTexture && info.playerSkinSprite.getTexture() != playerTexture.get())
            {
                info.playerSkinSprite.setTexture(*playerTexture);
                float desiredSize = 64.f;
                float scale = desiredSize / std::max(playerTexture->getSize().x, playerTexture->getSize().y);
                info.playerSkinSprite.setScale(scale, scale);
                float boardWidth = 490.f;
                info.playerSkinSprite.setPosition(START_X + boardWidth - desiredSize - 10.f, 20.f + VERTICAL_SPACING * playerInfos.size());
            }
        }
    }
}

void InGameHUD::render(sf::RenderWindow &window)
{
    window.draw(separatorSprite);

    window.draw(titleText);

    for (const auto &[playerId, info] : playerInfos)
    {
        window.draw(info.boardSprite);    
        window.draw(info.nicknameText);
        window.draw(info.scoreText);
        window.draw(info.healthText);
        window.draw(info.playerSkinSprite);     
    }
}
