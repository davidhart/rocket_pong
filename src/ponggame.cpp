#include "ponggame.h"
#include "renderqueue.h"
#include "vectormath.h"
#include "rendertarget.h"
#include "shader.h"

using namespace Rocket;

Shader* CreatePaddleShader(Renderer* renderer)
{
    char vertSource[] =
        "#version 140\n"
        "#extension GL_ARB_explicit_attrib_location : enable\n"
        "uniform mat4 u_proj;\n"
        "layout(location=0)in vec3 i_pos;\n"
        "layout(location=1)in vec2 i_uv;\n"
        "out float v_v;\n"
        "void main() {\n"
        "gl_Position = u_proj * vec4(i_pos.x, i_pos.y, 0.0, 1.0);\n"
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

void PongGame::Startup(GameServices* services)
{
    GameView* view = services->GameView();
    view->SetIsResizable(true);
    view->AddSizeObserver(this);

    Renderer* renderer = services->Renderer();
    ivec2 rtSize = renderer->GetPrimaryRenderTarget()->GetSize();

    float aspect = (float)rtSize.x / (float)rtSize.y;

    m_starfield.Init(renderer, 80, aspect);
    
    m_paddleShader = CreatePaddleShader(renderer);

    for (int i = 0; i < NumPlayers; ++i)
    {
        m_paddles[i].Init(renderer, m_paddleShader, 25.0f, 25, 20, aspect);
    }

    m_mainQueue = renderer->CreateRenderQueue("main", 0);
    m_mainQueue->SetClearColorEnabled(true);
    m_mainQueue->SetClearDepthEnabled(true);
}

void PongGame::Shutdown(GameServices* services)
{
    Renderer* renderer = services->Renderer();
    
    m_starfield.Release(renderer);

    for (int i = 0; i < NumPlayers; ++i)
    {
        m_paddles[i].Release(renderer);
    }

    renderer->ReleaseShader(m_paddleShader);

    renderer->ReleaseRenderQueue(m_mainQueue);
}

void PongGame::Update(float dt)
{
    m_starfield.Update(dt);


    m_mainQueue->Draw(m_starfield.GetDrawBinding(), m_starfield.GetMaterial());

    for (int i = 0; i < NumPlayers; ++i)
    {
        m_mainQueue->Draw(m_paddles[i].GetDrawBinding(), m_paddles[i].GetMaterial());
    }
}

void PongGame::GameViewResized(const ivec2& size)
{
    float aspect = (float)size.x / (float)size.y;
    m_starfield.SetAspectRatio(aspect);

    for (int i = 0; i < NumPlayers; ++i)
        m_paddles[i].SetAspectRatio(aspect);
}
