#include "TextRenderer.hpp"

#include "ResourceManager.hpp"

#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include <tuple>


TextRenderer::TextRenderer(const GLShaderProgram& shaderProgram)
    : m_shaderProgram(shaderProgram)
{
    // configure VAO/VBO for texture quads
    glGenVertexArrays(1, &m_quadVAO);
    glGenBuffers(1, &m_quadVBO);

    glBindVertexArray(m_quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void TextRenderer::Load(const char* fontPath, ui32 fontSize)
{
    // init FreeType library
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "ERROR::TextRenderer::Load::FreeType: Couldn't init FreeType library" << std::endl;
    }
    // load font
    FT_Face face;
    if (FT_New_Face(ft, fontPath, 0, &face)) {
        std::cerr << "ERROR::TextRenderer::Load::FreeType: Failed to load font" << std::endl;
    }
    // set font size (setting the width=0 lets the face dynamically calculate it based on the height)
    FT_Set_Pixel_Sizes(face, 0, fontSize);

    // NOTE: Disable byte alignment restriction. ???????????????????????????????????????????????????
    //  Is there really no other way, although it doesn't matter probably, this class is shit anyway
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    
    m_glyphs.reserve(128);
    // for the first 128 ASCII characters, pre-load/compile their characters and store them
    for (unsigned char c = 0; c < 128; ++c) {
        // load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "ERROR::TextRenderer::Load::FreeType: Failed to load Glyph '" << c << "'\n";
            continue;
        }
        const auto* glyphPtr = face->glyph;
        // generate texture
        // NOTE: Also dogshit. GLTexture class was made so we don't use it, good one
        ui32 textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                     glyphPtr->bitmap.width, glyphPtr->bitmap.rows,
                     0, GL_RED, GL_UNSIGNED_BYTE, glyphPtr->bitmap.buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // store glyph
        m_glyphs[c] = { .size = glm::ivec2(glyphPtr->bitmap.width, glyphPtr->bitmap.rows),
                        .bearing = glm::ivec2(glyphPtr->bitmap_left, glyphPtr->bitmap_top),
                        .advance = static_cast<ui32>(glyphPtr->advance.x),
                        .textureID = textureID };

        if (m_maxBearingHeight < glyphPtr->bitmap_top) {
            m_maxBearingHeight = glyphPtr->bitmap_top;
        }
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void TextRenderer::Render(const std::string_view text, f32 x, f32 y, f32 scale, glm::vec3 color /*= glm::vec3(1.0f)*/)
{
    m_shaderProgram.Bind();
    m_shaderProgram.SetFloat3("u_textColor", color);
    glActiveTexture(GL_TEXTURE0); // NOTE: Why do I need to call it?
    glBindVertexArray(m_quadVAO);

    for (const auto c : text) {
        const auto& glyph = m_glyphs[c];

        const auto xPos = x + glyph.bearing.x * scale;
        const auto yPos = y + (m_maxBearingHeight - glyph.bearing.y) * scale;

        const auto width = glyph.size.x * scale;
        const auto height = glyph.size.y * scale;

        const f32 vertices[6][4] = {
            {        xPos, yPos + height, 0.0f, 1.0f },
            {xPos + width,          yPos, 1.0f, 0.0f },
            {        xPos,          yPos, 0.0f, 0.0f },

            {        xPos, yPos + height, 0.0f, 1.0f },
            {xPos + width, yPos + height, 1.0f, 1.0f },
            {xPos + width,          yPos, 1.0f, 0.0f },
        };

        glBindTexture(GL_TEXTURE_2D, glyph.textureID);
        glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (glyph.advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
