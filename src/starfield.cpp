#include "starfield.h"
#include "drawbinding.h"
#include "buffer.h"
#include "shader.h"
#include "rendertarget.h"

const float DistanceScale = 2.0f;

using namespace Rocket;

struct Starfield_QuadVert
{
    Rocket::vec3 Pos_Lifetime;
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
        vec3 pos = vec3(stars[i].Pos.x, stars[i].Pos.y, stars[i].Lifetime);

        buffer[i * 4 + 0].Pos_Lifetime = pos + vec3(-pointSize, -pointSize, 0);
        buffer[i * 4 + 1].Pos_Lifetime = pos + vec3(pointSize, -pointSize, 0);
        buffer[i * 4 + 2].Pos_Lifetime = pos + vec3(pointSize, pointSize, 0);
        buffer[i * 4 + 3].Pos_Lifetime = pos + vec3(-pointSize, pointSize, 0);
    }
}

Shader* CreateShader(Renderer* renderer)
{
    char vertSource[] =
        "#version 140\n"
        "#extension GL_ARB_explicit_attrib_location : enable\n"
        "uniform mat4 u_proj;\n"
        "layout(location=0)in vec3 i_posxy_lifetime;\n"
        "layout(location=1)in vec2 i_uv;\n"
        "out float v_lifetime;\n"
        "void main() {\n"
        "gl_Position = u_proj * vec4(i_posxy_lifetime.x, i_posxy_lifetime.y, 0.0, 1.0);\n"
        "v_lifetime = i_posxy_lifetime.z;\n"
        "}\n";

    char fragSource[] =
        "#version 140\n"
        "#extension GL_ARB_explicit_attrib_location : enable\n"
        "in float v_lifetime;\n"
        "out vec4 f_frag;\n"
        "layout(location = 0) out vec4 frag; \n"
        "void main() {\n"
        "float a = smoothstep(0.1, 0.5, v_lifetime) * 1 - smoothstep(0.75, 1, v_lifetime);\n"
        "frag = vec4(a,a,a,a);\n"
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

void Starfield::Init(Renderer* renderer, int numStars, float aspectRatio)
{
    m_stars.resize(numStars);
    m_vertbuffer = renderer->CreateBuffer(numStars * sizeof(Starfield_QuadVert) * 4, nullptr);

    std::uniform_real_distribution<float> normalDistrib(0.0f, 1.0f);

    for (int i = 0; i < numStars; ++i)
    {
        m_stars[i].Lifetime = normalDistrib(m_randGenerator);
        m_stars[i].Direction = PickRandomDirection();
        m_stars[i].Pos = m_stars[i].Direction * m_stars[i].Lifetime * DistanceScale;
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
            0, VB_TYPE_FLOAT, DB_COMPONENTS_3, m_vertbuffer, 0, sizeof(Starfield_QuadVert)
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

    ivec2 size = renderer->GetPrimaryRenderTarget()->GetSize();
    
    SetAspectRatio(aspectRatio);
}

void Starfield::SetAspectRatio(float aspectRatio)
{
    m_material->GetParameters()->SetMat4("u_proj", mat4::Ortho(-(float)aspectRatio, (float)aspectRatio, -1, 1, -1, 1));
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
        m_stars[i].Pos = m_stars[i].Direction * life * DistanceScale;
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