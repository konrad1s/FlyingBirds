#pragma once

#include <vector>
#include <cstdint>
#include "EventBus.h"
#include "Events.h"

class ScoreboardManager
{
public:
    ScoreboardManager(EventBus &eventBus)
    {
        eventBus.subscribe<Events::PlayerDiedEvent>([this](const Events::PlayerDiedEvent &evt)
                                                    { onPlayerDied(evt.playerId); });
    }

    void reset()
    {
        deadPlayersOrder.clear();
    }

    void onPlayerDied(uint32_t playerId)
    {
        deadPlayersOrder.push_back(playerId);
    }

    const std::vector<uint32_t> &getDeadPlayersOrder() const
    {
        return deadPlayersOrder;
    }

private:
    std::vector<uint32_t> deadPlayersOrder;
};
