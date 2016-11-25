#pragma once
#ifndef PONGGAME_PADDLEGEOMETRY_INCLUDED
#define PONGGAME_PADDLEGEOMETRY_INCLUDED

namespace Rocket
{
    class Renderer;
    class DrawBinding;
    class Buffer;
}

class PaddleGeometry
{
public:
    PaddleGeometry();
    void Init(Rocket::Renderer* renderer, float arcAngle, int arcDivisions, int capDivisions);
    void Release(Rocket::Renderer* renderer);
    
    Rocket::DrawBinding* GetDrawBinding();

private:

    Rocket::Buffer* m_vertbuffer;
    Rocket::Buffer* m_indexbuffer;
    Rocket::DrawBinding* m_drawBinding;
};

#endif