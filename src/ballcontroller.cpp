#include "ballcontroller.h"
#include "renderer.h"
#include "shader.h"
#include "renderqueue.h"

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

void BallController::Init(Renderer *renderer, const mat4& projection)
{
    m_ballshader = CreateBallShader(renderer);
    
    m_ballGeometry.Init(renderer, 150, 0.1f);
    
    for (int i = 0; i < NumBalls; ++i)
    {
        m_balls[i].Init(&m_ballGeometry, m_ballshader, projection);
    }
}

void BallController::Release(Renderer* renderer)
{
    for (int i = 0; i < NumBalls; ++i)
    {
        m_balls[i].Release();
    }
    
    m_ballGeometry.Release(renderer);
    
    renderer->ReleaseShader(m_ballshader);
}

void BallController::SetProjection(const mat4& projection)
{
    for (int i = 0; i < NumBalls; ++i)
    {
        m_balls[i].SetProjection(projection);
    }
}

void BallController::Update(float dt)
{
    for (int i = 0; i < NumBalls; ++i)
    {
        m_balls[i].Update(dt);
    }
}

void BallController::Draw(RenderQueue* queue)
{
    for (int i = 0; i < NumBalls; ++i)
    {
        queue->Draw(m_balls[i].GetDrawBinding(), m_balls[i].GetMaterial());
    }
}
