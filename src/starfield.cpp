#include "starfield.h"
#include "drawbinding.h"
#include "buffer.h"
#include "shader.h"

using namespace Rocket;

struct Starfield_QuadVert
{
    Rocket::vec2 Pos;
    Rocket::vec2 Uv;
};

void SetBufferUVs(Starfield_QuadVert* buffer, int count)
{
    for (int i = 0; i < count; ++i)
    {
        buffer[i * 4 + 0].Uv = vec2(0, 0);
        buffer[i * 4 + 1].Uv = vec2(1, 0);
        buffer[i * 4 + 2].Uv = vec2(1, 1);
        buffer[i * 4 + 3].Uv = vec2(0, 1);
    }
}

void SetBufferVerts(Starfield_QuadVert* buffer, Starfield_Star* stars, int count)
{
    const float pointSize = 0.01f;

    for (int i = 0; i < count; ++i)
    {
        vec2 pos = stars[i].Pos;

        buffer[i * 4 + 0].Pos = pos + vec2(-pointSize, -pointSize);
        buffer[i * 4 + 1].Pos = pos + vec2(pointSize, -pointSize);
        buffer[i * 4 + 2].Pos = pos + vec2(pointSize, pointSize);
        buffer[i * 4 + 3].Pos = pos + vec2(-pointSize, pointSize);
    }
}

Shader* CreateShader(Renderer* renderer)
{
    char vertSource[] =
        "#version 140\n"
        "#extension GL_ARB_explicit_attrib_location : enable\n"
        "layout(location=0)in vec2 i_pos;\n"
        "layout(location=1)in vec2 i_uv;\n"
        "void main() {\n"
        "gl_Position = vec4(i_pos.x, i_pos.y, 0.0, 1.0);\n"
        "}\n";

    char fragSource[] =
        "#version 140\n"
        "#extension GL_ARB_explicit_attrib_location : enable\n"
        "out vec4 f_frag;\n"
        "layout(location = 0) out vec4 frag; \n"
        "void main() {\n"
        "frag = vec4(1,1,1,1);\n"
        "}\n";
    
    ShaderDef shaderDef;
    shaderDef.vert = vertSource;
    shaderDef.vertSize = sizeof(vertSource);
    shaderDef.frag = fragSource;
    shaderDef.fragSize = sizeof(fragSource);

    return renderer->CreateShader(shaderDef);
}

Starfield::Starfield() :
    m_vertbuffer(nullptr),
    m_indexbuffer(nullptr)
{
}

void Starfield::Init(Renderer* renderer, int numStars)
{
    m_stars.resize(numStars);
    m_vertbuffer = renderer->CreateBuffer(numStars * sizeof(Starfield_QuadVert) * 4, nullptr);

    std::uniform_real_distribution<float> normalDistrib(0.0f, 1.0f);

    for (int i = 0; i < numStars; ++i)
    {
        m_stars[i].Lifetime = normalDistrib(m_randGenerator);
        m_stars[i].Direction = PickRandomDirection();
        m_stars[i].Pos = m_stars[i].Direction * m_stars[i].Lifetime;
    }

    Starfield_QuadVert* buffer = reinterpret_cast<Starfield_QuadVert*>(m_vertbuffer->Map(Buffer::MAP_WRITE_ONLY));
    SetBufferUVs(buffer, (int)m_stars.size());
    SetBufferVerts(buffer, m_stars.data(), (int)m_stars.size());
    m_vertbuffer->Unmap();

    // Create fixed list of quad indices
    std::vector<short> indices(numStars * 6);

    for (int i = 0; i < numStars; ++i)
    {
        short b = (short)i * 4;
        indices[i * 6 + 0] = b + 0;
        indices[i * 6 + 1] = b + 1;
        indices[i * 6 + 2] = b + 2;

        indices[i * 6 + 3] = b + 0;
        indices[i * 6 + 4] = b + 2;
        indices[i * 6 + 5] = b + 3;
    }

    m_indexbuffer = renderer->CreateBuffer(indices.size() * sizeof(short), indices.data());
    
    IndexBinding indexBinding;
    indexBinding.buffer = m_indexbuffer;
    indexBinding.offet = 0;
    indexBinding.type = IB_TYPE_UINT16;

    Starfield_QuadVert placeholder;

    VertexBinding vertexBinding[2] = 
    {
        {
            0, VB_TYPE_FLOAT, DB_COMPONENTS_2, m_vertbuffer, 0, sizeof(Starfield_QuadVert)
        },
        {
            1, VB_TYPE_FLOAT, DB_COMPONENTS_2, m_vertbuffer, sizeof(float)*2, sizeof(Starfield_QuadVert)
        }
    };

    DrawBindingDef drawBindingDef;
    drawBindingDef.vertexAttributes = vertexBinding;
    drawBindingDef.numVertexAttributes = 2;
    drawBindingDef.indexAttributes = &indexBinding;
    drawBindingDef.length = numStars * 6;

    m_drawBinding = renderer->CreateDrawBinding(drawBindingDef);

    m_shader = CreateShader(renderer);
    m_material = new Material(m_shader);
}

void Starfield::Release(Renderer* renderer)
{
    delete m_material;
    renderer->ReleaseShader(m_shader);
    renderer->ReleaseDrawBinding(m_drawBinding);
    renderer->ReleaseBuffer(m_vertbuffer);
    renderer->ReleaseBuffer(m_indexbuffer);
}

void Starfield::Update(float dt)
{
    for (size_t i = 0; i < m_stars.size(); ++i)
    {
        float life = m_stars[i].Lifetime;

        life += dt;

        if (life > 1)
        {
            life = life - floor(life);
            m_stars[i].Direction = PickRandomDirection();
        }

        m_stars[i].Lifetime = life;
        m_stars[i].Pos = m_stars[i].Direction * life;
    }

    // Update the mesh
    Starfield_QuadVert* buffer = reinterpret_cast<Starfield_QuadVert*>(m_vertbuffer->Map(Buffer::MAP_WRITE_ONLY));
    SetBufferVerts(buffer, m_stars.data(), (int)m_stars.size());
    m_vertbuffer->Unmap();
}

DrawBinding* Starfield::GetDrawBinding()
{
    return m_drawBinding;
}

Material* Starfield::GetMaterial()
{
    return m_material;
}

vec2 Starfield::PickRandomDirection()
{
    vec2 dir;

    std::uniform_real_distribution<float> distrib(-1.0f, 1.0f);

    for (int i = 0; i < 10; ++i)
    {
        dir.x = distrib(m_randGenerator);
        dir.y = distrib(m_randGenerator);

        float magSqr = dir.MagnitudeSqr();

        if (magSqr < 1.0f)
        {
            return dir / sqrt(magSqr);
        }
    }

    return dir.Normalized();
}