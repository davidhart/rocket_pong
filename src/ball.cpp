#include "ball.h"
#include "ballgeometry.h"
#include "material.h"

using namespace Rocket;

void Ball::Init(BallGeometry *geometry, Material* material, const mat4& projection)
{
    m_geometry = geometry;
    m_material = material;
    
    SetProjection(projection);
}

void Ball::SetProjection(const mat4& projection)
{
    m_material->SetMat4("u_proj", projection);
}

void Ball::SetPosition(const vec2& position)
{
    m_position = vec3(position, 0.0f);
}

void Ball::Update(float dt)
{
    m_material->SetMat4("u_model", mat4::Translate(m_position));
}

DrawBinding* Ball::GetDrawBinding()
{
    return m_geometry->GetDrawBinding();
}

Material* Ball::GetMaterial()
{
    return m_material;
}
