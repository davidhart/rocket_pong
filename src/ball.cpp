#include "ball.h"
#include "ballgeometry.h"
#include "material.h"
#include "renderer.h"

using namespace Rocket;

void Ball::Init(BallGeometry *geometry, Material* material, Renderer* renderer)
{
    m_properties.Model = renderer->GetShaderPropertyID("u_model");
    m_geometry = geometry;
    m_material = material;
}

void Ball::SetPosition(const vec2& position)
{
    m_position = vec3(position, 0.0f);
}

void Ball::Update(float dt)
{
    m_material->SetShaderMat4(m_properties.Model, mat4::Translate(m_position));
}

DrawBinding* Ball::GetDrawBinding()
{
    return m_geometry->GetDrawBinding();
}

Material* Ball::GetMaterial()
{
    return m_material;
}
