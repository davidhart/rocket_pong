#pragma once
#ifndef PONGGAME_BALLCONTROLLER_INCLUDED
#define PONGGAME_BALLCONTROLLER_INCLUDED

#include "vectormath.h"
#include "ballgeometry.h"

namespace Rocket
{
    class Renderer;
    class Shader;
    class RenderQueue;
    class Material;
}

struct Ball
{
    Rocket::vec2 Position;
    Rocket::vec2 Velocity;
};

class BallController
{
public:
    BallController();
    void Init(Rocket::Renderer* renderer);
    void Release(Rocket::Renderer* renderer);
    
    void Update(float dt);
    void Draw(Rocket::RenderQueue* queue);
    
private:
    
    bool FindCollision(Ball& ball, float dt, float* at, Rocket::vec2* normal);
    
    struct
    {
        int Model;
    } m_properties;
    
    Rocket::Shader* m_ballshader;
    
    static const int NumBalls = 1;
    BallGeometry m_ballGeometry;
    Ball m_balls[NumBalls];
    
    Rocket::Material* m_materials[NumBalls];
};

#endif
