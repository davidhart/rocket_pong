#pragma once
#ifndef PONGGAME_STARFIELD_INCLUDED
#define PONGAGME_STARFIELD_INCLUDED

#include "renderer.h"
#include "vectormath.h"
#include "material.h"
#include <vector>
#include <random>

struct Starfield_Star
{
    float Lifetime;
    Rocket::vec2 Direction;
    Rocket::vec2 Pos;
};

class Starfield
{
public:
    Starfield();

    void Init(Rocket::Renderer* renderer, int numStars);

    void Release(Rocket::Renderer* renderer);

    void Update(float dt);

    Rocket::DrawBinding* GetDrawBinding();
    Rocket::Material* GetMaterial();
    
private:
    Rocket::vec2 PickRandomDirection();

    Rocket::Buffer* m_vertbuffer;
    Rocket::Buffer* m_indexbuffer;
    Rocket::DrawBinding* m_drawBinding;
    Rocket::Shader* m_shader;
    Rocket::Material* m_material;
    
    std::vector<Starfield_Star> m_stars;
    std::mt19937 m_randGenerator;
};

#endif
