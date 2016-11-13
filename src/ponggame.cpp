#include "ponggame.h"
#include "renderqueue.h"
#include "vectormath.h"

using namespace Rocket;

PongGame::PongGame() :
    m_renderer(nullptr)
{

}
PongGame::~PongGame()
{
    m_starfield.Release(m_renderer);

    m_renderer->ReleaseRenderQueue(m_mainQueue);
}

void PongGame::InitGraphics(Rocket::Renderer* renderer)
{
    m_renderer = renderer;
    m_starfield.Init(renderer, 80);

    m_mainQueue = renderer->CreateRenderQueue("main", 0);
    m_mainQueue->SetClearColorEnabled(true);
    m_mainQueue->SetClearDepthEnabled(true);
}

void PongGame::InitView(Rocket::GameView* view)
{

}

void PongGame::Update(float dt)
{
    m_starfield.Update(dt);

    m_mainQueue->Draw(m_starfield.GetDrawBinding(), m_starfield.GetMaterial());
}
