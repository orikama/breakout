#pragma once

#include "core.hpp"


class GLBackend
{
public:
    enum class GLBlendFactor : ui32
    {
        One = 1,
        SrcAlpha = 0x0302,
        OneMinusSrcAlpha = 0x0303
    };


    static void Init();

    static void SetViewport(i32 x, i32 y, i32 width, i32 height);
    static void SetClearColor(f32 r, f32 g, f32 b, f32 a);
    static void SetBlendFunction(GLBlendFactor source, GLBlendFactor destination);

    static void Clear(ui32 mask);

    static void DrawArrays(i32 count);
};
