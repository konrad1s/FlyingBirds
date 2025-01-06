#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Component.h"

namespace Components
{
    class Animation : public Component
    {
    public:
        Animation() : timePerFrame(0.06f), currentTime(0.f), currentFrame(0)
        {
        }

        void addFrame(const std::shared_ptr<sf::Texture> &frame)
        {
            frames.push_back(frame);
        }

        void setTimePerFrame(float t)
        {
            timePerFrame = t;
        }

        void update(float deltaTime)
        {
            if (frames.empty())
                return;

            currentTime += deltaTime;
            if (currentTime >= timePerFrame)
            {
                currentTime = 0.f;
                currentFrame = (currentFrame + 1) % frames.size();
            }
        }

        void applyToSprite(sf::Sprite &sprite)
        {
            if (frames.empty())
                return;

            sprite.setTexture(*frames[currentFrame], true);
            auto bounds = sprite.getLocalBounds();
            sprite.setOrigin(bounds.width * 0.5f, bounds.height * 0.5f);
        }

    private:
        std::vector<std::shared_ptr<sf::Texture>> frames;
        float timePerFrame;
        float currentTime;
        std::size_t currentFrame;
    };
}
