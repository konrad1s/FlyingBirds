#pragma once
#include <cstdint>
#include "network.pb.h"

namespace Events
{
    struct ClientConnectedEvent
    {
        uint32_t clientId;
    };

    struct ClientDisconnectedEvent
    {
        uint32_t clientId;
    };

    struct ClientMessageEvent
    {
        uint32_t clientId;
        network::ClientToServer c2sMessage;
    };
}
