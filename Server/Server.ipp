template <typename MsgType>
void Server::sendToClient(ClientId clientId, const MsgType &msg)
{
    auto it = clients.find(clientId);
    if (it != clients.end())
    {
        it->second->sendMessage(msg);
    }
    else
    {
        Logger::warning("sendToClient: client {} not found", clientId);
    }
}

template <typename MsgType>
void Server::broadcast(const MsgType &msg)
{
    for (auto &[clientId, handler] : clients)
    {
        handler->sendMessage(msg);
    }
}