#include "GLShaderProgram.hpp"

#include <iostream>
#include <memory>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>


auto _createShader(const char* shaderSource, const GLenum shaderType)   -> GLuint;


GLShaderProgram::GLShaderProgram()
{
    //std::cout << "default GLShaderProgram() constructor called\n";
    m_programID = 0;
}

GLShaderProgram::GLShaderProgram(const char* vertSource, const char* fragSource, const char* geomSource /*= nullptr*/)
{
    //std::cout << "GLShaderProgram() constructor called\n";

    const auto vertID = _createShader(vertSource, GL_VERTEX_SHADER);
    const auto fragID = _createShader(fragSource, GL_FRAGMENT_SHADER);
    const auto geomID = geomSource != nullptr ? _createShader(geomSource, GL_GEOMETRY_SHADER) : GLuint(0);

    m_programID = glCreateProgram();
    glAttachShader(m_programID, vertID);
    glAttachShader(m_programID, fragID);
    if (geomSource != nullptr) {
        glAttachShader(m_programID, geomID);
    }

    glLinkProgram(m_programID);

    glDeleteShader(vertID);
    glDeleteShader(fragID);
    if (geomSource != nullptr) {
        glDeleteShader(geomID);
    }
}

//GLShaderProgram::~GLShaderProgram()
//{
//    // call glUseProgram(0) ?
//    glDeleteProgram(m_programID);
//}


GLShaderProgram::GLShaderProgram(GLShaderProgram&& copy)
{
    //std::cout << "GLShaderProgram() move copy constructor called\n";
    m_programID = copy.m_programID;
}
GLShaderProgram& GLShaderProgram::operator=(GLShaderProgram&& copy)
{
    //std::cout << "GLShaderProgram() move assignment called\n";
    m_programID = copy.m_programID;
    return *this;
}


void GLShaderProgram::Bind() const
{
    glUseProgram(m_programID);
}


void GLShaderProgram::SetInt1(const std::string& name, const i32 value) const
{
    auto location = glGetUniformLocation(m_programID, name.c_str());
    glUniform1i(location, value);
}

void GLShaderProgram::SetInt1v(const std::string& name, const i32 count, const i32 intArray[]) const
{
    auto location = glGetUniformLocation(m_programID, name.c_str());
    glUniform1iv(location, count, intArray);
}

void GLShaderProgram::SetFloat1(const std::string& name, const f32 value) const
{
    auto location = glGetUniformLocation(m_programID, name.c_str());
    glUniform1f(location, value);
}

void GLShaderProgram::SetFloat1v(const std::string& name, const i32 count, const f32 floatArray[]) const
{
    auto location = glGetUniformLocation(m_programID, name.c_str());
    glUniform1fv(location, count, floatArray);
}

void GLShaderProgram::SetFloat2(const std::string& name, const glm::vec2& vector) const
{
    auto location = glGetUniformLocation(m_programID, name.c_str());
    glUniform2f(location, vector.x, vector.y);
}

// NOTE: Should be std::array I guess
void GLShaderProgram::SetFloat2v(const std::string& name, const i32 count, const f32 vectorArray[][2]) const
{
    auto location = glGetUniformLocation(m_programID, name.c_str());
    glUniform2fv(location, count, (f32*)vectorArray);
}

void GLShaderProgram::SetFloat3(const std::string& name, const glm::vec3& vector) const
{
    auto location = glGetUniformLocation(m_programID, name.c_str());
    glUniform3f(location, vector.x, vector.y, vector.z);
}

void GLShaderProgram::SetFloat4(const std::string& name, const glm::vec4& vector) const
{
    auto location = glGetUniformLocation(m_programID, name.c_str());
    glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
}

void GLShaderProgram::SetMatrix4(const std::string& name, const glm::mat4& matrix) const
{
    auto location = glGetUniformLocation(m_programID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}


GLuint _createShader(const char* shaderSource, const GLenum shaderType)
{
    auto shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, &shaderSource, nullptr);
    glCompileShader(shaderID);

    return shaderID;
}
