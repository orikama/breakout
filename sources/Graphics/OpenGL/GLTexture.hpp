#pragma once

#include "core.hpp"


class GLTexture
{
public:
    GLTexture() = default;
    GLTexture(const ui32 width, const ui32 height, bool alpha, const ui8* data);
    //~GLTexture();

    /*GLTexture(const GLTexture&) = delete;
    GLTexture(GLTexture&&);
    GLTexture& operator=(const GLTexture&);
    GLTexture& operator=(GLTexture&&);*/

    void Bind() const;

//private:
    ui32 m_textureID;
    /*ui32 m_width;
    ui32 m_height;*/
};
