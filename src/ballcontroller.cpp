#include "ballcontroller.h"
#include "renderer.h"
#include "shader.h"
#include "renderqueue.h"
#include "material.h"

using namespace Rocket;

Shader* CreateBallShader(Renderer* renderer)
{
    char vertSource[] =
    "#version 140\n"
    "#extension GL_ARB_explicit_attrib_location : enable\n"
    "uniform mat4 u_proj;\n"
    "uniform mat4 u_model;\n"
    "layout(location=0)in vec3 i_pos;\n"
    "layout(location=1)in vec2 i_uv;\n"
    "out float v_v;\n"
    "void main() {\n"
    "gl_Position = u_proj * u_model * vec4(i_pos.x, i_pos.y, 0.0, 1.0);\n"
    "v_v = i_uv.y;\n"
    "}\n";
    
    char fragSource[] =
    "#version 140\n"
    "#extension GL_ARB_explicit_attrib_location : enable\n"
    "in float v_v;\n"
    "out vec4 f_frag;\n"
    "layout(location = 0) out vec4 frag; \n"
    "void main() {\n"
    "float a = v_v;\n"
    "frag = vec4(a,a,a,a);\n"
    "}\n";
    
    ShaderDef shaderDef;
    shaderDef.vert = vertSource;
    shaderDef.vertSize = sizeof(vertSource);
    shaderDef.frag = fragSource;
    shaderDef.fragSize = sizeof(fragSource);
    
    return renderer->CreateShader(shaderDef);
}

BallController::BallController() :
    m_ballshader(nullptr)
{
}

void BallController::Init(Renderer *renderer)
{
    m_ballshader = CreateBallShader(renderer);
    m_properties.Model = renderer->GetShaderPropertyID("u_model");
    
    m_ballGeometry.Init(renderer, 150, 0.1f);
    
    for (int i = 0; i < NumBalls; ++i)
    {
        m_materials[i] = renderer->CreateMaterial(m_ballshader);
    }
    
    m_balls[0].Position = vec2(0.25, 0.75f);
    m_balls[0].Velocity = vec2(1.0f, 0.25f);
}

void BallController::Release(Renderer* renderer)
{
    for (int i = 0; i < NumBalls; ++i)
    {
        renderer->ReleaseMaterial(m_materials[i]);
    }
    
    m_ballGeometry.Release(renderer);
    
    renderer->ReleaseShader(m_ballshader);
}

void BallController::Update(float dt)
{
    for (int i = 0; i < NumBalls; ++i)
    {
        Ball& ball = m_balls[i];
        float step = dt;
        vec2 normal = vec2(0, 0);
        
        while (FindCollision(ball, step, &step, &normal))
        {
            ball.Position += ball.Velocity * step;
            ball.Velocity -=  2 * vec2::Dot(ball.Velocity, normal) * normal;
        }
        
        ball.Position += ball.Velocity * step;
        
        m_materials[i]->SetShaderMat4(m_properties.Model, mat4::Translate(vec3(ball.Position, 0.0f)));
    }
}

void BallController::Draw(RenderQueue* queue)
{
    for (int i = 0; i < NumBalls; ++i)
    {
        queue->Draw(m_ballGeometry.GetDrawBinding(), m_materials[i]);
    }
}

bool BallController::FindCollision(Ball& ball, float, float* at, vec2* normal)
{
    if (ball.Position.Magnitude() > 1 && vec2::Dot(ball.Velocity, ball.Position) > 0)
    {
        *at = 0.0f;
        *normal = ball.Position.Normalized();
        return true;
    }
    return false;
}
