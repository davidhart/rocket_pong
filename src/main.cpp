#include "bootstrap.h"
#include "ponggame.h"

using namespace Rocket;

int main(int, char**)
{
    Bootstrap bootstrap;
    bootstrap.Start("Pong");
    PongGame game;
    game.InitView(bootstrap.GetGameView());
    game.InitGraphics(bootstrap.GetRenderer());
    return bootstrap.Run(&game);
}
