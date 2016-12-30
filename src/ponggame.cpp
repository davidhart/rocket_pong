#include "ponggame.h"
#include "renderqueue.h"
#include "vectormath.h"
#include "rendertarget.h"
#include "shader.h"

using namespace Rocket;

mat4 CalculateProjection(const ivec2& size)
{
    if (size.x > size.y)
    {
        float aspect = (float)size.x / (float)size.y;
        return mat4::Ortho(-aspect, aspect, -1, 1, -1, 1);
    }
    else
    {
        float aspect = (float)size.y / (float)size.x;
        return mat4::Ortho(-1, 1, -aspect, aspect, -1, 1);
    }
}

void PongGame::Startup(GameServices* services)
{
    GameView* view = services->GameView();
    view->SetIsResizable(true);
    view->AddSizeObserver(this);

    Renderer* renderer = services->Renderer();
    m_properties.Projection = renderer->GetShaderPropertyID("u_proj");
    m_renderer = renderer;
    
    ivec2 rtSize = renderer->GetPrimaryRenderTarget()->GetSize();
    
    renderer->SetShaderMat4(m_properties.Projection, CalculateProjection(rtSize));
    
    m_starfield.Init(renderer, 80);
    m_paddleController.Init(services);
    m_ballController.Init(renderer);
    
    m_mainQueue = renderer->CreateRenderQueue("main", 0);
    m_mainQueue->SetClearColorEnabled(true);
    m_mainQueue->SetDepthTestEnabled(false);
}

void PongGame::Shutdown(GameServices* services)
{
    Renderer* renderer = services->Renderer();
    
    m_starfield.Release(renderer);
    m_paddleController.Release(services);

    renderer->ReleaseRenderQueue(m_mainQueue);
}

void PongGame::Update(float dt)
{
    m_starfield.Update(dt);
    m_paddleController.Update(dt);
    m_ballController.Update(dt);

    m_mainQueue->Draw(m_starfield.GetDrawBinding(), m_starfield.GetMaterial());
    m_paddleController.Draw(m_mainQueue);
    m_ballController.Draw(m_mainQueue);
}

void PongGame::GameViewResized(const ivec2& size)
{
    m_renderer->SetShaderMat4(m_properties.Projection, CalculateProjection(size));
}
