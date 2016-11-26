#pragma once
#ifndef PONGGAME_BALLCONTROLLER_INCLUDED
#define PONGGAME_BALLCONTROLLER_INCLUDED

#include "vectormath.h"
#include "ball.h"
#include "ballgeometry.h"

namespace Rocket
{
    class Renderer;
    class Shader;
    class RenderQueue;
}

class BallController
{
public:
    BallController();
    void Init(Rocket::Renderer* renderer, const Rocket::mat4& projection);
    void Release(Rocket::Renderer* renderer);
    void SetProjection(const Rocket::mat4& projection);
    
    void Update(float dt);
    void Draw(Rocket::RenderQueue* queue);
    
private:
    Rocket::Shader* m_ballshader;
    
    static const int NumBalls = 4;
    BallGeometry m_ballGeometry;
    Ball m_balls[NumBalls];
};

#endif
