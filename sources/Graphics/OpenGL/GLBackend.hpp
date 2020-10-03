#pragma once

#include "core.hpp"


class GLBackend
{
public:
    static void Init();

    static void SetViewport(i32 x, i32 y, i32 width, i32 height);
    static void SetClearColor(f32 r, f32 g, f32 b, f32 a);

    static void Clear(ui32 mask);

    static void DrawArrays(i32 count);
};
