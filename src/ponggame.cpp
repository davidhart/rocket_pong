#include "ponggame.h"
#include "renderqueue.h"
#include "vectormath.h"

using namespace Rocket;

void PongGame::Startup(Rocket::GameServices* services)
{
    Renderer* renderer = services->Renderer();

    m_starfield.Init(renderer, 80);

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
