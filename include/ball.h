#pragma once
#ifndef PONGGAME_BALL_INCLUDED
#define PONGGAME_BALL_INCLUDED

#include "vectormath.h"

namespace Rocket
{
    class Shader;
    class Material;
    class DrawBinding;
    class Renderer;
}

class BallGeometry;

class Ball
{
public:
    void Init(BallGeometry* geometry, Rocket::Material* material, Rocket::Renderer* renderer);
    void SetPosition(const Rocket::vec2& position);
    void Update(float dt);
    
    Rocket::DrawBinding* GetDrawBinding();
    Rocket::Material* GetMaterial();
    
private:
    struct
    {
        int Model;
    } m_properties;
    
    Rocket::vec3 m_position;
    
    BallGeometry* m_geometry;
    Rocket::Material* m_material;
};

#endif
