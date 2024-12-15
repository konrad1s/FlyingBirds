template <typename MessageType,
          std::enable_if_t<std::is_base_of_v<google::protobuf::Message, MessageType>, bool>>
int NetworkHandler::sendMessage(const MessageType &msg)
{
    std::string buffer;

    if (!msg.SerializeToString(&buffer))
        return -1;

    msgSize_t size = static_cast<msgSize_t>(buffer.size());

    if (socket.send(&size, sizeof(size)))
        return -1;

    if (socket.send(buffer.data(), buffer.size()) != sf::Socket::Done)
        return -1;

    return 0;
}

template <typename MessageType,
          std::enable_if_t<std::is_base_of_v<google::protobuf::Message, MessageType>, bool>>
int NetworkHandler::receiveMessage(const MessageType &msg)
{
    msgSize_t size = 0;
    std::size_t received = 0;
    auto status = socket.receive(&size, sizeof(size), received);

    if (status == sf::Socket::NotReady)
        return -1;

    if (status != sf::Socket::Done || received != sizeof(size))
        return -1;

    std::string buffer(size, '\0');
    std::size_t totalReceived = 0;

    /* TODO: Handle async/timeout */
    while (totalReceived < size)
    {
        std::size_t chunk = 0;
        auto res = socket.receive(&buffer[totalReceived], size - totalReceived, chunk);
        if (res != sf::Socket::Done)
            return -1;
        totalReceived += chunk;
    }

    if (!msg.ParseFromString(buffer))
        return -1;

    return 0;
}

