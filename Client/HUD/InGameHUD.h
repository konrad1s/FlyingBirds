#pragma once

#include "HUDBase.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <mutex>
#include <unordered_map>
#include "Player.h"

class GameWorld;

class InGameHUD : public HUDBase
{
public:
    InGameHUD(sf::RenderWindow &window);
    ~InGameHUD() override = default;

    void handleEvent(sf::RenderWindow& window, const sf::Event& event) override;
    void update(float deltaTime) override {}
    void update(GameWorld &world, float deltaTime);
    void render(sf::RenderWindow& window) override;

private:
    struct PlayerInfo
    {
        sf::Sprite boardSprite;
        sf::Text nicknameText;
        sf::Text scoreText;
        sf::Text healthText;
        sf::Sprite playerSkinSprite;
    };

    void initializeHud(sf::RenderWindow& window);
    void setupSeparator(sf::RenderWindow& window);
    void setupTitleText();
    void setupBoardTexture();

    const float START_X = 80.f;
    const float VERTICAL_SPACING = 100.f;

    std::shared_ptr<sf::Font> font;
    std::shared_ptr<sf::Texture> separatorTexture;
    std::shared_ptr<sf::Texture> boardTexture;

    sf::Text titleText;
    sf::Sprite separatorSprite;

    std::unordered_map<uint32_t, PlayerInfo> playerInfos;
};
