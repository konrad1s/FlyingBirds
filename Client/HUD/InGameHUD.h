#pragma once

#include "HUDBase.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <unordered_map>
#include "Player.h"

class GameWorld;

class InGameHUD : public HUDBase
{
public:
    explicit InGameHUD(sf::RenderWindow &window);
    ~InGameHUD() override = default;

    void handleEvent(sf::RenderWindow &window, const sf::Event &event) override;
    void update(float deltaTime) override {}
    void update(GameWorld &world, float deltaTime);
    void render(sf::RenderWindow &window) override;
    void setTimeRemaining(float time);

private:
    struct PlayerInfo
    {
        sf::Sprite boardSprite;
        sf::Text nicknameText;
        sf::Text scoreText;
        sf::Sprite playerSkinSprite;
        sf::Sprite protectionIcon;
        sf::Sprite speedBoostIcon;
    };

    void initializeHud(sf::RenderWindow &window);
    void setupSeparator(sf::RenderWindow &window);
    void setupTitleText();

    PlayerInfo createPlayerInfo(const Player *playerPtr, std::size_t index) const;
    void updatePlayerInfo(PlayerInfo &info, const Player *playerPtr, std::size_t index);

    static constexpr float START_X = 80.f;
    static constexpr float VERTICAL_SPACING = 100.f;

    std::shared_ptr<sf::Font> font;
    std::shared_ptr<sf::Texture> separatorTexture;
    std::shared_ptr<sf::Texture> boardTexture;
    std::shared_ptr<sf::Texture> protectionTexture;
    std::shared_ptr<sf::Texture> speedBoostTexture;

    sf::Text titleText;
    sf::Sprite separatorSprite;

    float timeRemaining = 0.0f;
    sf::Text timeText;

    std::unordered_map<uint32_t, PlayerInfo> playerInfos;
};
