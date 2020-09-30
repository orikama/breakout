#include "GLTexture.hpp"

#include <glad/glad.h>

#include <iostream> // TODO: REMOVE


GLTexture::GLTexture(const ui32 width, const ui32 height, bool alpha, const ui8* data)
{
    std::cout << "GLTexture() constructor called\n";

    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    // TODO: use modern opengl: glTextureStorage2d(), glTextureParameteri(), glTextureSubImage2D()
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (alpha) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

// NOTE: I guess can be more effective if I make a static method DeleteTextures() that will delete all textures at once,
//GLTexture::~GLTexture()
//{
//    glDeleteTextures(1, &textureID);
//}


//GLTexture::GLTexture(GLTexture&& copy)
//{
//    std::cout << "GLTexture() move copy constructor called\n";
//    m_textureID = copy.m_textureID;
//}
//GLTexture& GLTexture::operator=(const GLTexture& copy)
//{
//    std::cout << "GLTexture() move assignment called\n";
//    m_textureID = copy.m_textureID;
//    return *this;
//}
//GLTexture& GLTexture::operator=(GLTexture&& copy)
//{
//    std::cout << "GLTexture() move assignment called\n";
//    m_textureID = copy.m_textureID;
//    return *this;
//}


void GLTexture::Bind() const
{
    // TODO: use glBindTextureUnit()
    glBindTexture(GL_TEXTURE_2D, m_textureID);
}
