#include <SFML/Graphics.hpp>
#include "Server.h"

int main()
{
    const ConfigServer config;
    Server server(config);

    server.run();

    return 0;
}