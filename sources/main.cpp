#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Game.hpp"

#include <iostream>


constexpr ui32 kScreenWidth = 800;
constexpr ui32 kScreenHeight = 600;


void OpenGLMessageCallback(
    unsigned source,
    unsigned type,
    unsigned id,
    unsigned severity,
    int length,
    const char* message,
    const void* userParam)
{
    std::cerr << message << std::endl;
}


void keyCallback(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mode);


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);

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

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(OpenGLMessageCallback, nullptr);

    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);

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
        /*if (currentTime - lastPrintTime >= 1.0f) {
            printf("FPS: %d %f ms\n", frameCount, 1000.0 / frameCount);
            frameCount = 0;
            lastPrintTime += 1.0f;
        }*/

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
        }
    }
}
