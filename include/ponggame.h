#pragma once
#ifndef PONGGAME_INCLUDED
#define PONGGAME_INCLUDED

#include "game.h"
#include "gameview.h"
#include "renderer.h"

#include "starfield.h"
#include "paddle.h"

class PongGame : public Rocket::Game, public Rocket::IGameViewSizeObserver
{
public:
    virtual void Startup(Rocket::GameServices* services);
    virtual void Shutdown(Rocket::GameServices* services);

    void Update(float dt);

    virtual void GameViewResized(const Rocket::ivec2& size);

private:
    static const int NumPlayers = 1;

    Starfield m_starfield;
    Paddle m_paddles[NumPlayers];

    Rocket::Shader* m_paddleShader;

    Rocket::RenderQueue* m_mainQueue;
};

#endif