#include "ballgeometry.h"
#include "renderer.h"
#include "vectormath.h"
#include "drawbinding.h"
#include <vector>

using namespace Rocket;

struct Ball_Vert
{
    vec2 Pos;
    vec2 Uv;
};

BallGeometry::BallGeometry() :
    m_vertexbuffer(nullptr),
    m_indexbuffer(nullptr),
    m_drawbinding(nullptr)
{
}

void BallGeometry::Init(Renderer* renderer, int numSegments, float radius)
{
    const int numVerts = numSegments + 1;
    std::vector<Ball_Vert> verts(numVerts);
    
    const float anglePerSegment = (float)ROCKET_PI * 2.0f / numSegments;
    
    for (int i = 0; i < numSegments; ++i)
    {
        float angle = anglePerSegment * i;
        verts[i].Pos = vec2(sin(angle), cos(angle)) * radius;
        verts[i].Uv = vec2(0.0f, 1.0f);
    }
    
    verts[numSegments].Pos = vec2::Zero();
    verts[numSegments].Uv = vec2::Zero();
    
    
    const int numIndices = (numSegments + 1) * 3;
    std::vector<short> indices(numIndices);
    
    int id = 0;
    
    for (short i = 1; i < numSegments; ++i)
    {
        indices[id++] = i;
        indices[id++] = i-1;
        indices[id++] = (short)numSegments;
    }
    
    indices[id++] = 0;
    indices[id++] = (short)numSegments - 1;
    indices[id++] = (short)numSegments;
    
    m_vertexbuffer = renderer->CreateBuffer(numVerts * sizeof(Ball_Vert), verts.data());
    m_indexbuffer = renderer->CreateBuffer(numIndices * sizeof(short), indices.data());
    
    IndexBinding indexBinding;
    indexBinding.buffer = m_indexbuffer;
    indexBinding.offet = 0;
    indexBinding.type = IB_TYPE_UINT16;
    
    VertexBinding vertexBinding[2] =
    {
        {
            0, VB_TYPE_FLOAT, DB_COMPONENTS_2, m_vertexbuffer, 0, sizeof(Ball_Vert)
        },
        {
            1, VB_TYPE_FLOAT, DB_COMPONENTS_2, m_vertexbuffer, sizeof(float) * 2, sizeof(Ball_Vert)
        }
    };
    
    DrawBindingDef drawBindingDef;
    drawBindingDef.vertexAttributes = vertexBinding;
    drawBindingDef.numVertexAttributes = 2;
    drawBindingDef.indexAttributes = &indexBinding;
    drawBindingDef.length = indices.size();
    
    m_drawbinding = renderer->CreateDrawBinding(drawBindingDef);
}

void BallGeometry::Release(Renderer* renderer)
{
    renderer->ReleaseDrawBinding(m_drawbinding);
    renderer->ReleaseBuffer(m_vertexbuffer);
    renderer->ReleaseBuffer(m_indexbuffer);
}

DrawBinding* BallGeometry::GetDrawBinding()
{
    return m_drawbinding;
}
