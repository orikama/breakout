#pragma once

#include "Graphics/OpenGL/GLShaderProgram.hpp"
#include "Graphics/OpenGL/GLTexture.hpp"

#include <string>
#include <unordered_map>


class ResourceManager
{
public:
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;

    static void LoadShaderProgram(const std::string& name, const char* vertPath, const char* fragPath, const char* geomPath = nullptr);
    static void LoadTexture(const std::string& name, bool alpha, const char* texturePath);

    static const GLShaderProgram& GetShaderProgram(const std::string& name);
    static const GLTexture& GetTexture(const std::string& name);

private:
    inline static std::unordered_map<std::string, GLShaderProgram> m_shaderPrograms;
    inline static std::unordered_map<std::string, GLTexture> m_textures;
};
