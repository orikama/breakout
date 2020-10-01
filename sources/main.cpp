#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Game.hpp"

#include <iostream>


constexpr ui32 kScreenWidth = 800;
constexpr ui32 kScreenHeight = 600;


#ifdef GL_DEBUG_CONTEXT
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
#endif // GL_DEBUG_CONTEXT


void keyCallback(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mode);


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);
#ifdef GL_DEBUG_CONTEXT
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif // GL_DEBUG_CONTEXT

    auto window = glfwCreateWindow(kScreenWidth, kScreenHeight, "Breakout", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window\n";
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    glViewport(0, 0, kScreenWidth, kScreenHeight);
#ifdef GL_DEBUG_CONTEXT
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
#endif // GL_DEBUG_CONTEXT
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Game g_breakout(kScreenWidth, kScreenHeight);
    glfwSetWindowUserPointer(window, &g_breakout);
    glfwSetKeyCallback(window, keyCallback);

    f32 deltaTime = 0.0f;
    f32 previousTime = 0.0f;

    f32 lastPrintTime = 0.0f;
    i32 frameCount = 0;


    while (!glfwWindowShouldClose(window)) {
        f32 currentTime = glfwGetTime();
        deltaTime = currentTime - previousTime;
        previousTime = currentTime;

        ++frameCount;
        if (currentTime - lastPrintTime >= 1.0f) {
            printf("FPS: %d %f ms\n", frameCount, 1000.0 / frameCount);
            frameCount = 0;
            lastPrintTime += 1.0f;
        }

        glfwPollEvents();

        g_breakout.ProcessInput(deltaTime);
        g_breakout.Update(deltaTime);
        g_breakout.Render(glfwGetTime());

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}


void keyCallback(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (key >= 0 && key < 1024) {
        Game& game = *static_cast<Game*>(glfwGetWindowUserPointer(window));
        if (action == GLFW_PRESS) {
            game.m_keys[key] = true;
        }
        else if (action == GLFW_RELEASE) {
            game.m_keys[key] = false;
            game.m_keysProcessed[key] = false;
        }
    }
}
