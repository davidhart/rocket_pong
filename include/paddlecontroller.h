#pragma once
#ifndef PONGGAME_PADDLECONTROLLER_INCLUDED
#define PONGGAME_PADDLECONTROLLER_INCLUDED

#include "vectormath.h"
#include "paddle.h"
#include "paddlegeometry.h"

namespace Rocket
{
    class GameServices;
    class Shader;
    class RenderQueue;
}

class PaddleController
{
public:

    void Init(Rocket::GameServices* services, const Rocket::mat4& projection);
    void Release(Rocket::GameServices* services);

    void SetProjection(const Rocket::mat4& projection);

    void Draw(Rocket::RenderQueue* queue);
    void Update(float dt);

private:
    static const int NumPlayers = 4;

    PaddleGeometry m_geometry;
    Paddle m_paddle[NumPlayers];

    Rocket::Shader* m_shader;
};

#endif