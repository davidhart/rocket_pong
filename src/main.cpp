#include "ponggame.h"

using namespace Rocket;

int main(int, char**)
{
    PongGame game;
    return Game::Bootstrap(&game);
}
