#pragma once

#include "HUDBase.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>

class GameEndHUD : public HUDBase
{
public:
    struct ScoreEntry
    {
        uint32_t playerId;
        float mass;
        std::string nickname;
        std::shared_ptr<sf::Texture> playerTexture;
    };

    GameEndHUD(sf::RenderWindow &window);
    ~GameEndHUD() override = default;

    void handleEvent(sf::RenderWindow &window, const sf::Event &event) override;

    void update(float deltaTime) override {}

    void setScoreboard(const std::vector<ScoreEntry> &scoreboard);

    void render(sf::RenderWindow &window) override;

private:
    struct ScoreRow
    {
        sf::Sprite bgSprite;
        sf::Text rowText;
        sf::Sprite playerSprite;
    };

    void initializeHUD(sf::RenderWindow &window);

private:
    sf::RenderWindow &m_window;
    std::shared_ptr<sf::Font> font;
    std::shared_ptr<sf::Texture> boardTexture;

    sf::Text titleText;
    std::vector<ScoreRow> rows;
};
