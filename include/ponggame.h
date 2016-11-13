#pragma once
#ifndef PONGGAME_INCLUDED
#define PONGGAME_INCLUDED

#include "game.h"
#include "gameview.h"
#include "renderer.h"

#include "starfield.h"

class PongGame : public Rocket::Game
{
public:
    PongGame();
    virtual ~PongGame();

    void InitGraphics(Rocket::Renderer* renderer);
    void InitView(Rocket::GameView* view);

    void Update(float dt);

private:
    Rocket::Renderer* m_renderer;
    Starfield m_starfield;
    Rocket::RenderQueue* m_mainQueue;
};

#endif