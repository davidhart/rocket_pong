#include "paddlecontroller.h"
#include "game.h"
#include "renderer.h"
#include "shader.h"
#include "renderqueue.h"

using namespace Rocket;

Shader* CreatePaddleShader(Renderer* renderer)
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

void PaddleController::Init(GameServices* services)
{
    Renderer* renderer = services->Renderer();

    m_shader = CreatePaddleShader(renderer);

    m_geometry.Init(services->Renderer(), 40.0f, 25, 20);

    for (int i = 0; i < NumPlayers; ++i)
    {
        Material* material = renderer->CreateMaterial(m_shader);
        m_paddle[i].Init(&m_geometry, material);
    }

    for (int i = 1; i < NumPlayers; ++i)
    {
        m_paddle[i].SetRotation((float)i * (float)ROCKET_PI * 2.0f / (float)NumPlayers);
    }
}

void PaddleController::Release(GameServices* services)
{
    Renderer* renderer = services->Renderer();

    for (int i = 0; i < NumPlayers; ++i)
    {
        renderer->ReleaseMaterial(m_paddle[i].GetMaterial());
    }

    m_geometry.Release(renderer);
    renderer->ReleaseShader(m_shader);
}

void PaddleController::Draw(RenderQueue* queue)
{
    for (int i = 0; i < NumPlayers; ++i)
    {
        queue->Draw(m_paddle[i].GetDrawBinding(), m_paddle[i].GetMaterial());
    }
}

void PaddleController::Update(float dt)
{
    for (int i = 0; i < NumPlayers; ++i)
    {
        m_paddle[i].Update(dt);
    }
}
