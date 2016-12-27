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

    void Init(PaddleGeometry* geometry, Rocket::Material* material, const Rocket::mat4& projection);
    void Update(float dt);
    
    void SetRotation(float rotation);
    void SetProjection(const Rocket::mat4& projection);

    Rocket::DrawBinding* GetDrawBinding();
    Rocket::Material* GetMaterial();


private:

    float m_rotation;

    PaddleGeometry* m_geometry;
    Rocket::Material* m_material;
};

#endif
