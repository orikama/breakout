cmake_minimum_required(VERSION 3.17)
project(breakout CXX)


if(DEFINED ENV{CMAKE_TOOLCHAIN_FILE} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    message("YEP")
    set(CMAKE_TOOLCHAIN_FILE $ENV{CMAKE_TOOLCHAIN_FILE})
endif()

if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
    message("NOP")
    set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
endif()


#find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)


set(breakout_SRC_DIR "${PROJECT_SOURCE_DIR}/sources")
set(breakout_SRC ${breakout_SRC_DIR}/main.cpp
                 ${breakout_SRC_DIR}/core.hpp
                 ${breakout_SRC_DIR}/Game.hpp
                 ${breakout_SRC_DIR}/Game.cpp
                 ${breakout_SRC_DIR}/GameLevel.hpp
                 ${breakout_SRC_DIR}/GameLevel.cpp
                 ${breakout_SRC_DIR}/GameObject.hpp
                 ${breakout_SRC_DIR}/GameObject.cpp
                 ${breakout_SRC_DIR}/BallObject.hpp
                 ${breakout_SRC_DIR}/BallObject.cpp
                 ${breakout_SRC_DIR}/PowerUp.hpp
                 ${breakout_SRC_DIR}/PowerUp.cpp
                 ${breakout_SRC_DIR}/ResourceManager.hpp
                 ${breakout_SRC_DIR}/ResourceManager.cpp
                 ${breakout_SRC_DIR}/SpriteRenderer.hpp
                 ${breakout_SRC_DIR}/SpriteRenderer.cpp
                 ${breakout_SRC_DIR}/ParticleGenerator.hpp
                 ${breakout_SRC_DIR}/ParticleGenerator.cpp
                 ${breakout_SRC_DIR}/PostProcessor.hpp
                 ${breakout_SRC_DIR}/PostProcessor.cpp)

set(graphics_SRC_DIR "${breakout_SRC_DIR}/Graphics")

set(graphics_gl_SRC_DIR "${graphics_SRC_DIR}/OpenGL")
set(graphics_gl_SRC ${graphics_gl_SRC_DIR}/GLShaderProgram.hpp
                    ${graphics_gl_SRC_DIR}/GLShaderProgram.cpp
                    ${graphics_gl_SRC_DIR}/GLTexture.hpp
                    ${graphics_gl_SRC_DIR}/GLTexture.cpp)


set(third_party_SRC_DIR "${breakout_SRC_DIR}/third_party")
set(third_party_SRC ${third_party_SRC_DIR}/stb/stb_image.h
                    ${third_party_SRC_DIR}/stb/stb_image.cpp)


add_executable(breakout ${breakout_SRC} ${graphics_gl_SRC} ${third_party_SRC})
target_include_directories(breakout PRIVATE ${breakout_SRC_DIR} ${third_party_SRC_DIR}) #${Vulkan_INCLUDE_DIRS})
target_link_libraries(breakout #${Vulkan_LIBRARIES}
                               glfw
                               glm
                               glad::glad)
target_compile_features(breakout PRIVATE cxx_std_20)
