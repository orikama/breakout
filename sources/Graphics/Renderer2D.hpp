#pragma once

#include "core.hpp"

#include "OpenGL/GLBuffer.hpp"
#include "OpenGL/GLShaderProgram.hpp"
#include "OpenGL/GLTexture.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>


class Renderer2D
{
public:
    enum class BlendFactor : ui32
    {
        One = 1,
        SrcAlpha = 0x0302,
        OneMinusSrcAlpha = 0x0303
    };


    static void Init(const glm::mat4& projection);

    // NOTE: This methods probably shouldn't be there
    static void SetViewport(i32 x, i32 y, i32 width, i32 height);
    static void SetClearColor(f32 r, f32 g, f32 b, f32 a);
    static void SetBlendFunction(BlendFactor source, BlendFactor destination);
    static void Clear(ui32 mask);

    static void DrawQuad(const GLTexture& texture,
                         glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.f),
                         f32 rotate = 0.0f, glm::vec4 color = glm::vec4(1.0f));

private:
    inline static GLShaderProgram m_shaderProgram;
    //inline static GLTexture m_texture;
    inline static GLBuffer m_quadBuffer;
};
