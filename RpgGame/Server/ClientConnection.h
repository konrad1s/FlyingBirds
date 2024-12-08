#pragma once

#include <SFML/Network.hpp>

class ClientConnection
{
public:
    ClientConnection();

    sf::TcpSocket& getSocket()
    {
        return socket;
    }

private:
    sf::TcpSocket socket;
};