#define _USE_MATH_DEFINES
#include <cmath>

#include "paddlegeometry.h"
#include "renderer.h"
#include "drawbinding.h"
#include "vectormath.h"
#include <vector>

using namespace Rocket;

struct Paddle_Vert
{
    vec2 Pos;
    vec2 Uv;
};

PaddleGeometry::PaddleGeometry()
{

}

void PaddleGeometry::Init(Renderer* renderer, float arcAngle, int arcDivisions, int capDivisions)
{
    const int numArcVerts = arcDivisions * 3;
    const int numCapVerts = (capDivisions - 1) * 2;
    const int numVerts = numArcVerts + numCapVerts;
    std::vector<Paddle_Vert> verts(numVerts);

    const float startAngle = -arcAngle * 0.5f;
    const float angleSteps = arcAngle / arcDivisions;

    for (int i = 0; i < arcDivisions; ++i)
    {
        float angle = DegreesToRadians(startAngle + angleSteps * i);
        vec2 direction = vec2(std::cos(angle), std::sin(angle));

        verts[i].Pos = direction;
        verts[i].Uv = vec2(0, 1);
        verts[i + arcDivisions].Pos = direction * 0.9f;
        verts[i + arcDivisions].Uv = vec2(0, 0);
        verts[i + arcDivisions * 2].Pos = direction * 0.8f;
        verts[i + arcDivisions * 2].Uv = vec2(0, 1);
    }

    vec2 startMid = verts[arcDivisions].Pos;
    vec2 startLeftBasis = verts[0].Pos - startMid;
    vec2 startForwardBasis = vec2(-startLeftBasis.y, startLeftBasis.x);

    vec2 endMid = verts[arcDivisions *2 - 1].Pos;
    vec2 endLeftBasis = verts[arcDivisions - 1].Pos - endMid;
    vec2 endForwardBasis = vec2(-endLeftBasis.y, endLeftBasis.x);
    
    const float startCapAngle = 0.0f;
    const float capAngleSteps = M_PI / capDivisions;

    for (int i = 0; i < capDivisions - 1; ++i)
    {
        float angle = startCapAngle + capAngleSteps * (i + 1);
        vec2 dir = vec2(std::cos(angle), std::sin(angle));

        verts[numArcVerts + i].Pos = startMid + (startLeftBasis * dir.x) + (startForwardBasis * -dir.y);
        verts[numArcVerts + i].Uv = vec2(0, 1);

        verts[numArcVerts + (capDivisions - 1) + i].Pos = endMid + (endLeftBasis * dir.x) + (endForwardBasis * dir.y);
        verts[numArcVerts + (capDivisions - 1) + i].Uv = vec2(0, 1);
    }
    
    const int numArcIndices = (arcDivisions - 1) * 12;
    const int numCapIndices = capDivisions * 6;
    const int numIndices = numArcIndices + numCapIndices;
    std::vector<short> indices(numIndices);

    for (int i = 0; i < arcDivisions - 1; ++i)
    {
        short a0 = i;
        short a1 = i + (short)arcDivisions;
        short a2 = i + (short)arcDivisions * 2;
        short b0 = (i + 1);
        short b1 = (i + 1) + (short)arcDivisions;
        short b2 = (i + 1) + (short)arcDivisions * 2;

        int o = i * 12;
        indices[o] = a0;
        indices[++o] = b0;
        indices[++o] = a1;
        indices[++o] = b0;
        indices[++o] = b1;
        indices[++o] = a1;

        indices[++o] = a1;
        indices[++o] = b1;
        indices[++o] = a2;
        indices[++o] = b1;
        indices[++o] = b2;
        indices[++o] = a2;
    }

    short startCapMid = arcDivisions;
    short startCapPrev = 0;
    short startCapIndex = numArcIndices;

    short endCapMid = arcDivisions * 2 - 1;
    short endCapPrev = arcDivisions - 1;
    short endCapIndex = numArcIndices + capDivisions * 3;

    for (int i = 0; i < capDivisions - 1; ++i)
    {
        short startCap = numArcVerts + i;
        indices[startCapIndex++] = startCap;
        indices[startCapIndex++] = startCapPrev;
        indices[startCapIndex++] = startCapMid;
        startCapPrev = startCap;

        short endCap = numArcVerts + (capDivisions - 1) + i;
        indices[endCapIndex++] = endCapPrev;
        indices[endCapIndex++] = endCap;
        indices[endCapIndex++] = endCapMid;
        endCapPrev = endCap;
    }

    indices[startCapIndex++] = arcDivisions * 2;
    indices[startCapIndex++] = startCapPrev;
    indices[startCapIndex++] = startCapMid;

    indices[endCapIndex++] = endCapPrev;
    indices[endCapIndex++] = arcDivisions * 3 - 1;
    indices[endCapIndex++] = endCapMid;

    m_vertbuffer = renderer->CreateBuffer(verts.size() * sizeof(Paddle_Vert), verts.data());
    m_indexbuffer = renderer->CreateBuffer(indices.size() * sizeof(short), indices.data());

    IndexBinding indexBinding;
    indexBinding.buffer = m_indexbuffer;
    indexBinding.offet = 0;
    indexBinding.type = IB_TYPE_UINT16;

    VertexBinding vertexBinding[2] =
    {
        {
            0, VB_TYPE_FLOAT, DB_COMPONENTS_2, m_vertbuffer, 0, sizeof(Paddle_Vert)
        },
        {
            1, VB_TYPE_FLOAT, DB_COMPONENTS_2, m_vertbuffer, sizeof(float) * 2, sizeof(Paddle_Vert)
        }
    };

    DrawBindingDef drawBindingDef;
    drawBindingDef.vertexAttributes = vertexBinding;
    drawBindingDef.numVertexAttributes = 2;
    drawBindingDef.indexAttributes = &indexBinding;
    drawBindingDef.length = indices.size();

    m_drawBinding = renderer->CreateDrawBinding(drawBindingDef);
}

void PaddleGeometry::Release(Renderer* renderer)
{
    renderer->ReleaseDrawBinding(m_drawBinding);
    renderer->ReleaseBuffer(m_vertbuffer);
    renderer->ReleaseBuffer(m_indexbuffer);
}

DrawBinding* PaddleGeometry::GetDrawBinding()
{
    return m_drawBinding;
}