#include <SFML/Graphics.hpp>
#include "GameManager.h"

int main()
{
    const ConfigServer config;
    GameManager gameManager(config);

    gameManager.run();

    return 0;
}