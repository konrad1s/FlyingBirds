#include "GameEndHUD.h"
#include "ResourceManager.h"
#include "Logger.h"
#include <algorithm>
#include <sstream>

GameEndHUD::GameEndHUD(sf::RenderWindow &window)
    : m_window(window)
{
    initializeHUD(window);
}

void GameEndHUD::initializeHUD(sf::RenderWindow &window)
{
    auto &resMgr = ResourceManager::getInstance();

    try
    {
        font = resMgr.acquire<sf::Font>("arial", "fonts/arial.ttf");
    }
    catch (const std::exception &e)
    {
        Logger::error("Failed to load font for GameEndHUD: {}", e.what());
    }

    try
    {
        boardTexture = resMgr.acquire<sf::Texture>("board", "hud/board.png");
    }
    catch (const std::exception &e)
    {
        Logger::error("Failed to load board texture for GameEndHUD: {}", e.what());
    }

    if (font)
    {
        titleText.setFont(*font);
        titleText.setString("Final Scoreboard");
        titleText.setCharacterSize(56);
        titleText.setFillColor(sf::Color::Black);
    }
}

void GameEndHUD::handleEvent(sf::RenderWindow &, const sf::Event &)
{
}

void GameEndHUD::setScoreboard(const std::vector<ScoreEntry> &scoreboard)
{
    rows.clear();
    rows.reserve(scoreboard.size());

    float scoreboardWidth = 600.f;
    float scoreboardRowH = 70.f;
    float scoreboardHeight = scoreboardRowH * scoreboard.size();

    float windowWidth = static_cast<float>(m_window.getSize().x);
    float windowHeight = static_cast<float>(m_window.getSize().y);

    float scoreboardX = (windowWidth - scoreboardWidth) * 0.5f;
    float scoreboardY = (windowHeight - scoreboardHeight) * 0.5f;

    if (font)
    {
        sf::FloatRect titleBounds = titleText.getLocalBounds();
        titleText.setOrigin(titleBounds.width * 0.5f, 0.f);
        titleText.setPosition(windowWidth * 0.5f, scoreboardY - (titleBounds.height + 60.f));
    }

    for (size_t i = 0; i < scoreboard.size(); ++i)
    {
        const auto &entry = scoreboard[i];

        ScoreRow row;

        if (boardTexture)
        {
            row.bgSprite.setTexture(*boardTexture);

            auto bounds = row.bgSprite.getLocalBounds();
            if (bounds.width > 0.f && bounds.height > 0.f)
            {
                float scaleX = scoreboardWidth / bounds.width;
                float scaleY = scoreboardRowH / bounds.height;
                row.bgSprite.setScale(scaleX, scaleY);
            }
            row.bgSprite.setColor(sf::Color(255, 255, 255, 180));
            row.bgSprite.setPosition(scoreboardX, scoreboardY + static_cast<float>(i) * scoreboardRowH);
        }

        if (font)
        {
            row.rowText.setFont(*font);
            row.rowText.setCharacterSize(30);
            row.rowText.setFillColor(sf::Color::Black);

            std::ostringstream oss;
            oss << "Rank " << (i + 1)
                << " | " << entry.nickname
                << " | Score: " << static_cast<int>(entry.mass);

            row.rowText.setString(oss.str());
            row.rowText.setPosition(scoreboardX + 20.f,
                                    scoreboardY + static_cast<float>(i) * scoreboardRowH + 15.f);
        }

        if (entry.playerTexture)
        {
            row.playerSprite.setTexture(*entry.playerTexture);

            float desiredSize = 50.f;
            auto texSize = entry.playerTexture->getSize();
            if (texSize.x > 0 && texSize.y > 0)
            {
                float largestDim = static_cast<float>(std::max(texSize.x, texSize.y));
                float scale = desiredSize / largestDim;
                row.playerSprite.setScale(scale, scale);
            }

            float spriteX = scoreboardX + scoreboardWidth - 70.f;
            float spriteY = scoreboardY + static_cast<float>(i) * scoreboardRowH + 10.f;
            row.playerSprite.setPosition(spriteX, spriteY);
        }

        rows.push_back(std::move(row));
    }
}

void GameEndHUD::render(sf::RenderWindow &window)
{
    window.draw(titleText);

    for (auto &r : rows)
    {
        window.draw(r.bgSprite);
        window.draw(r.rowText);
        window.draw(r.playerSprite);
    }
}
