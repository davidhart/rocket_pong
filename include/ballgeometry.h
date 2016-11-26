#pragma once
#ifndef PONGGAME_BALLGEOMETRY_INCLUDED
#define PONGGAME_BALLGEOMETRY_INCLUDED

namespace Rocket
{
    class Renderer;
    class Buffer;
    class DrawBinding;
}

class BallGeometry
{
public:
    BallGeometry();
    void Init(Rocket::Renderer* renderer, int numSegments, float radius);
    void Release(Rocket::Renderer* renderer);
    Rocket::DrawBinding* GetDrawBinding();
    
private:
    Rocket::Buffer* m_vertexbuffer;
    Rocket::Buffer* m_indexbuffer;
    Rocket::DrawBinding* m_drawbinding;
};

#endif
