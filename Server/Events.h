#pragma once
#include <cstdint>
#include "network.pb.h"

namespace Events
{
    struct ClientConnectedEvent
    {
        uint32_t clientId;
        std::string nickname;

        ClientConnectedEvent(uint32_t id, const std::string&& nick)
            : clientId(id), nickname(std::move(nick)) {}
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
