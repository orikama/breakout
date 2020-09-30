#include "PostProcessor.hpp"

#include <glad/glad.h>

#include <iostream>


PostProcessor::PostProcessor(const GLShaderProgram& shaderProgram, ui32 width, ui32 height)
    : m_shaderProgram(shaderProgram)
    , m_texture(width, height, false, nullptr)
    , m_width(width)
    , m_height(height)
{
    glGenFramebuffers(1, &m_multisampledFBO);
    glGenFramebuffers(1, &m_FBO);
    glGenRenderbuffers(1, &m_RBO);

    glBindFramebuffer(GL_FRAMEBUFFER, m_multisampledFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_RBO);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "ERROR::PostProcessor: Failed to initialize MSFBO" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture.m_textureID, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "ERROR::PostProcessor: Failed to initialize FBO" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    initRenderData();

    constexpr auto offset = 1.0f / 300.0f;
    const f32 offsets[9][2] = {
        { -offset,  offset},    // top-left
        {    0.0f,  offset},    // top-center
        {  offset,  offset},    // top-right
        { -offset,    0.0f},    // center-left
        {    0.0f,    0.0f},    // center
        {  offset,    0.0f},    // center-right
        { -offset, -offset},    // bottom-left
        {    0.0f, -offset},    // bottom-center
        {  offset, -offset},    // bottom-right
    };

    const int edgeKernel[9] = {
        -1, -1, -1,
        -1,  8, -1,
        -1, -1, -1
    };

    const f32 blurKernel[9] = {
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
        2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
    };

    m_shaderProgram.Bind();
    m_shaderProgram.SetFloat2v("u_offsets", 9, offsets);
    m_shaderProgram.SetInt1v("u_edgeKernel", 9, edgeKernel);
    m_shaderProgram.SetFloat1v("u_blurKernel", 9, blurKernel);
}


void PostProcessor::BeginRender()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_multisampledFBO);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void PostProcessor::EndRender()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_multisampledFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO);
    glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessor::Render(f32 time)
{
    m_shaderProgram.Bind();
    m_shaderProgram.SetFloat1("u_time", time);
    m_shaderProgram.SetInt1("u_confuse", m_isConfuse);
    m_shaderProgram.SetInt1("u_chaos", m_isChaos);
    m_shaderProgram.SetInt1("u_shake", m_isShake);

    glActiveTexture(GL_TEXTURE0);
    m_texture.Bind();
    glBindVertexArray(m_quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}


// NOTE: The name has changed, but I still think I've seen this somewhere before
void PostProcessor::initRenderData()
{
    const f32 vertices[] = {
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,

        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f
    };

    GLuint vbo;
    glGenVertexArrays(1, &m_quadVAO);
    glGenBuffers(1, &vbo);

    glBindVertexArray(m_quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
