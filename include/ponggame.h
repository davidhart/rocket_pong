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
    virtual void Startup(Rocket::GameServices* services);
    virtual void Shutdown(Rocket::GameServices* services);

    void Update(float dt);

private:
    Starfield m_starfield;
    Rocket::RenderQueue* m_mainQueue;
};

#endif