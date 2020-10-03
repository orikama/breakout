#include "Renderer2D.hpp"

#include "OpenGL/GLBackend.hpp"
#include "ResourceManager.hpp"

#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>


GLBackend::GLBlendFactor blendFactorToBackend(Renderer2D::BlendFactor blendFactor);


void Renderer2D::Init(const glm::mat4& projection)
{
    GLBackend::Init();


    const f32 vertices[] = {
        -0.5f,  0.5f, 0.0f, 1.0f,
         0.5f, -0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f,  0.5f, 0.0f, 1.0f,
         0.5f,  0.5f, 1.0f, 1.0f,
         0.5f, -0.5f, 1.0f, 0.0f
    };

    m_quadBuffer = GLBuffer(sizeof(vertices), vertices);


    ResourceManager::LoadShaderProgram("quad", "shaders/quad.vert", "shaders/quad.frag");

    m_shaderProgram = ResourceManager::GetShaderProgram("quad");
    m_shaderProgram.Bind();
    m_shaderProgram.SetMatrix4("u_projection", projection);
    m_shaderProgram.SetInt1("u_texture", 0);
}


void Renderer2D::SetViewport(i32 x, i32 y, i32 width, i32 height)
{
    GLBackend::SetViewport(x, y, width, height);
}

void Renderer2D::SetClearColor(f32 r, f32 g, f32 b, f32 a)
{
    GLBackend::SetClearColor(r, g, b, a);
}

void Renderer2D::SetBlendFunction(BlendFactor source, BlendFactor destination)
{
    GLBackend::SetBlendFunction(blendFactorToBackend(source), blendFactorToBackend(destination));
}

void Renderer2D::Clear(ui32 mask)
{
    GLBackend::Clear(mask);
}


void Renderer2D::DrawQuad(const GLTexture& texture,
                          glm::vec2 position, glm::vec2 size /*= glm::vec2(10.0f, 10.f)*/,
                          f32 rotate /*= 0.0f*/, glm::vec4 color /*= glm::vec3(1.0f)*/)
{
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::translate(model, glm::vec3(position, 0.0f));

    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));

    m_shaderProgram.Bind();
    m_shaderProgram.SetMatrix4("u_model", model);
    m_shaderProgram.SetFloat4("u_color", color);

    m_quadBuffer.Bind();
    texture.Bind(0);

    GLBackend::DrawArrays(6);
}


GLBackend::GLBlendFactor blendFactorToBackend(Renderer2D::BlendFactor blendFactor)
{
    // NOTE: For now this will work
    return static_cast<GLBackend::GLBlendFactor>(blendFactor);
}
