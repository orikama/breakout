#pragma once

#include "core.hpp"


class GLBuffer
{
public:
    GLBuffer() = default;
    GLBuffer(ui32 size, const void* vertices);

    void Bind() const;

private:
    ui32 m_VAO;
    ui32 m_VBO;
};
