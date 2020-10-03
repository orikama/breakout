#include "GLBuffer.hpp"

#include <glad/glad.h>


// NOTE: Temporary solution
//  For now no vertext layout
GLBuffer::GLBuffer(ui32 size, const void* vertices)
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void GLBuffer::Bind() const
{
    glBindVertexArray(m_VAO);
}
