#include "paddle.h"
#include "material.h"
#include "shader.h"
#include "paddlegeometry.h"

using namespace Rocket;

Paddle::Paddle() :
    m_rotation(0.0f)
{
}

void Paddle::Init(Rocket::Shader* shader, PaddleGeometry* geometry, const mat4& projection)
{
    m_material = new Material(shader);
    m_parameters = m_material->GetParameters();
    m_geometry = geometry;
    
    SetProjection(projection);
}

void Paddle::Release()
{
    delete m_material;
}

void Paddle::Update(float dt)
{
    m_rotation += dt;
    m_parameters->SetMat4("u_model", mat4::AxisAngle(vec3::Forward(), m_rotation));
}

void Paddle::SetRotation(const float rotation)
{
    m_rotation = rotation;
}

void Paddle::SetProjection(const mat4& projection)
{
    m_parameters->SetMat4("u_proj", projection);
}

DrawBinding* Paddle::GetDrawBinding()
{
    return m_geometry->GetDrawBinding();
}

Material* Paddle::GetMaterial()
{
    return m_material;
}