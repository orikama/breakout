#include "GLBackend.hpp"

#include <glad/glad.h>

#include <iostream>


#ifdef GL_ENABLE_DEBUG_CONTEXT
void openGLMessageCallback(GLenum source, GLenum type,
                           ui32 id, GLenum severity, GLsizei length,
                           const char* message, const void* userParam)
{
    std::cerr << "Debug message (" << id << "): " << message;
    std::cerr << "\nSource: ";
    switch (source) {
        case GL_DEBUG_SOURCE_API: std::cerr << "API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: std::cerr << "Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cerr << "Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY: std::cerr << "Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION: std::cerr << "Application"; break;
        case GL_DEBUG_SOURCE_OTHER: std::cerr << "Other"; break;
    }
    std::cerr << "\nType: ";
    switch (type) {
        case GL_DEBUG_TYPE_ERROR: std::cerr << "Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cerr << "Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: std::cerr << "Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY: std::cerr << "Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE: std::cerr << "Performance"; break;
        case GL_DEBUG_TYPE_MARKER: std::cerr << "Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP: std::cerr << "Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP: std::cerr << "Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER: std::cerr << "Other"; break;
    }
    std::cerr << "\nSeverity: ";
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH: std::cerr << "high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM: std::cerr << "medium"; break;
        case GL_DEBUG_SEVERITY_LOW: std::cerr << "low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cerr << "notification"; break;
    }
    std::cerr << std::endl;
}
#endif // GL_ENABLE_DEBUG_CONTEXT


void GLBackend::Init()
{
#ifdef GL_ENABLE_DEBUG_CONTEXT
    {
        i32 flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(openGLMessageCallback, nullptr);

            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
        }
    }
#endif // GL_DEGL_ENABLE_DEBUG_CONTEXTBUG_CONTEXT

    // NOTE: Should be optional
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void GLBackend::SetViewport(i32 x, i32 y, i32 width, i32 height)
{
    glViewport(0, 0, width, height);
}

void GLBackend::SetClearColor(f32 r, f32 g, f32 b, f32 a)
{
    glClearColor(r, g, b, a);
}


void GLBackend::Clear(ui32 mask)
{
    glClear(mask);
}


void GLBackend::DrawArrays(i32 count)
{
    glDrawArrays(GL_TRIANGLES, 0, count);
}
