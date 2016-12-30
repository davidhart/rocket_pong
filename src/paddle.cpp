#include "paddle.h"
#include "paddlegeometry.h"
#include "renderer.h"

using namespace Rocket;

Paddle::Paddle() :
    m_rotation(0.0f)
{
}

void Paddle::Init(PaddleGeometry* geometry, Material* material, Renderer* renderer)
{
    m_properties.Model = renderer->GetShaderPropertyID("u_model");
    m_material = material;
    m_geometry = geometry;
}

void Paddle::Update(float dt)
{
    m_rotation += dt;
    m_material->SetShaderMat4(m_properties.Model, mat4::AxisAngle(vec3::Forward(), m_rotation));
}

void Paddle::SetRotation(const float rotation)
{
    m_rotation = rotation;
}

DrawBinding* Paddle::GetDrawBinding()
{
    return m_geometry->GetDrawBinding();
}

Material* Paddle::GetMaterial()
{
    return m_material;
}
