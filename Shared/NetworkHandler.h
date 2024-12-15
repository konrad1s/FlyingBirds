#pragma once

#include <SFML/Network.hpp>
#include <google/protobuf/message.h>
#include <type_traits>
#include <cstdint>
#include <string>

class NetworkHandler
{
public:
    NetworkHandler() = default;

    template <typename MessageType,
              std::enable_if_t<std::is_base_of_v<google::protobuf::Message, MessageType>, bool> = true>
    int sendMessage(const MessageType &msg);

    template <typename MessageType,
              std::enable_if_t<std::is_base_of_v<google::protobuf::Message, MessageType>, bool> = true>
    int receiveMessage(const MessageType &msg);

    sf::TcpSocket &getSocket();

private:
    typedef uint32_t msgSize_t;
    sf::TcpSocket socket;
};

#include "NetworkHandler.ipp"
