#pragma once

#include "core.hpp"
#include "Graphics/OpenGL/GLShaderProgram.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <string_view>
#include <unordered_map>


class TextRenderer
{
public:
    TextRenderer(const GLShaderProgram& shaderProgram);

    void Load(const char* fontPath, ui32 fontSize);

    void Render(const std::string_view text, f32 x, f32 y, f32 scale, glm::vec3 color = glm::vec3(1.0f));

private:
    struct Glyph
    {
        glm::ivec2 size;    // size of glyph
        glm::ivec2 bearing; // offset from baseline to left/top of glyph
        ui32 advance;       // horizontal offset to advance to next glyph
        ui32 textureID;
    };

    std::unordered_map<char, Glyph> m_glyphs;

    const GLShaderProgram& m_shaderProgram;
    ui32 m_quadVAO;
    ui32 m_quadVBO;

    i32 m_maxBearingHeight = 0;
};
