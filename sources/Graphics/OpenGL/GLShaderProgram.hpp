#pragma once

#include "core.hpp"

#include <string>
#include <glm/mat4x4.hpp>


class GLShaderProgram
{
public:
    GLShaderProgram();// = default;
    GLShaderProgram(const char* vertSource, const char* fragSource, const char* geomSource = nullptr);
    //~GLShaderProgram();

    //GLShaderProgram(const GLShaderProgram& copy) = delete;
    //GLShaderProgram(GLShaderProgram&& copy);
    //GLShaderProgram& operator=(const GLShaderProgram& copy) = delete;
    //GLShaderProgram& operator=(GLShaderProgram&& copy);

    void Bind() const;

    void SetInt1(const std::string& name, const i32 value) const;
    void SetInt1v(const std::string& name, const i32 count, const i32 intArray[]) const;
    void SetFloat1(const std::string& name, const f32 value) const;
    void SetFloat1v(const std::string& name, const i32 count, const f32 floatArray[]) const;
    void SetFloat2(const std::string& name, const glm::vec2& vector) const;
    void SetFloat2v(const std::string& name, const i32 count, const f32 vectorArray[][2]) const;
    void SetFloat3(const std::string& name, const glm::vec3& vector) const;
    void SetFloat4(const std::string& name, const glm::vec4& vector) const;
    void SetMatrix4(const std::string& name, const glm::mat4& matrix) const;

//private:
    ui32 m_programID;
};
