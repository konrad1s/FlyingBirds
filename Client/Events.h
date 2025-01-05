#pragma once

#include "network.pb.h"

namespace Events
{
    struct StateUpdateEvent
    {
        network::ServerToClient message;
    };

    struct WelcomeEvent
    {
        network::ServerToClient message;
    };

    struct GoodbyeEvent
    {
        network::ServerToClient message;
    };

    struct PlayButtonClickEvent
    {
        std::string ip;
        unsigned short port;
        std::string nickname;
    };
} // namespace Events
