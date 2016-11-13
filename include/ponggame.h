#pragma once
#ifndef PONGGAME_INCLUDED
#define PONGGAME_INCLUDED

#include "game.h"
#include "gameview.h"
#include "renderer.h"
#include "buffer.h"
#include "shader.h"
#include "texture.h"
#include "drawbinding.h"
#include "material.h"
#include "renderqueue.h"
#include "rendertarget.h"
#include "vectormath.h"
#include "input.h"

class PongGame : public Rocket::Game
{
public:
    PongGame();
    virtual ~PongGame();

    void InitGraphics(Rocket::Renderer* renderer);
    void InitView(Rocket::GameView* view);

    void Update(float dt);
};

#endif