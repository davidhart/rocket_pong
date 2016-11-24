#pragma once
#ifndef PONGGAME_PADDLE_INCLUDED
#define PONGGAME_PADDLE_INCLUDED

#include "renderer.h"
#include "material.h"

class Paddle
{
public:
    Paddle();
    void Init(Rocket::Renderer* renderer, Rocket::Shader* shader, float arcAngle, int arcDivisions, int capDivisions, float aspect);
    void Release(Rocket::Renderer* renderer);

    void SetAspectRatio(float aspectRatio);

    Rocket::Material* GetMaterial();
    Rocket::DrawBinding* GetDrawBinding();

private:

    Rocket::Buffer* m_vertbuffer;
    Rocket::Buffer* m_indexbuffer;
    Rocket::Material* m_material;
    Rocket::DrawBinding* m_drawBinding;
};

#endif