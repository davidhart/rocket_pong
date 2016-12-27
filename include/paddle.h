#pragma once
#ifndef PONGGAME_PADDLE_INCLUDED
#define PONGGAME_PADDLE_INCLUDED

#include "vectormath.h"
#include "material.h"

namespace Rocket
{
    class DrawBinding;
    class Material;
    class Shader;
    class ShaderParameters;
}

class PaddleGeometry;

class Paddle
{
public:
    Paddle();

    void Init(PaddleGeometry* geometry, Rocket::Material* material);
    void Update(float dt);
    
    void SetRotation(float rotation);

    Rocket::DrawBinding* GetDrawBinding();
    Rocket::Material* GetMaterial();


private:

    float m_rotation;

    PaddleGeometry* m_geometry;
    Rocket::Material* m_material;
};

#endif
