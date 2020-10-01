#pragma once

#include "core.hpp"
#include "Graphics/OpenGL/GLShaderProgram.hpp"
#include "Graphics/OpenGL/GLTexture.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>


class SpriteRenderer
{
public:
    SpriteRenderer(const GLShaderProgram& shader);

    /*SpriteRenderer(const SpriteRenderer&) = delete;
    SpriteRenderer(SpriteRenderer&&) = delete;
    SpriteRenderer& operator=(const SpriteRenderer&) = delete;
    SpriteRenderer& operator=(SpriteRenderer&&) = delete;*/

    void DrawSprite(const GLTexture& texture,
                    glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.f),
                    f32 rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f)) const;

private:
    void initRenderData();


    const GLShaderProgram& m_shaderProgram;
    ui32 m_quadVAO;
};
