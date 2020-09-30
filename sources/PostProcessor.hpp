#pragma once

#include "core.hpp"

#include "Graphics/OpenGL/GLShaderProgram.hpp"
#include "Graphics/OpenGL/GLTexture.hpp"
#include "SpriteRenderer.hpp"


class PostProcessor
{
public:
    const GLShaderProgram& m_shaderProgram;
    const GLTexture m_texture;
    ui32 m_width;
    ui32 m_height;
    bool m_isConfuse = false;
    bool m_isChaos = false;
    bool m_isShake = false;


    PostProcessor(const GLShaderProgram& shaderProgram, ui32 width, ui32 height);

    void BeginRender();
    void EndRender();
    void Render(f32 time);

private:
    void initRenderData();


    ui32 m_FBO;
    ui32 m_multisampledFBO;
    ui32 m_RBO; // Render buffer object
    ui32 m_quadVAO;
};
