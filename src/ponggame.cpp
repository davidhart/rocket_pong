#include "ponggame.h"
#include "renderqueue.h"
#include "vectormath.h"
#include "rendertarget.h"

using namespace Rocket;

void PongGame::Startup(Rocket::GameServices* services)
{
    GameView* view = services->GameView();
    view->SetIsResizable(true);
    view->AddSizeObserver(this);

    Renderer* renderer = services->Renderer();
    ivec2 rtSize = renderer->GetPrimaryRenderTarget()->GetSize();

    m_starfield.Init(renderer, 80, (float)rtSize.x / (float)rtSize.y);

    m_mainQueue = renderer->CreateRenderQueue("main", 0);
    m_mainQueue->SetClearColorEnabled(true);
    m_mainQueue->SetClearDepthEnabled(true);
}

void PongGame::Shutdown(Rocket::GameServices* services)
{
    Renderer* renderer = services->Renderer();
    
    m_starfield.Release(renderer);
    renderer->ReleaseRenderQueue(m_mainQueue);
}

void PongGame::Update(float dt)
{
    m_starfield.Update(dt);

    m_mainQueue->Draw(m_starfield.GetDrawBinding(), m_starfield.GetMaterial());
}

void PongGame::GameViewResized(const ivec2& size)
{
    m_starfield.SetAspectRatio((float)size.x / (float)size.y);
}
