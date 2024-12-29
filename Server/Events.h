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

    struct ClientDataUpdatedEvent
    {
        uint32_t clientId;
        network::ClientToServer message;
    };
}
