#include "ResourceManager.hpp"

#include "core.hpp"

#include "stb/stb_image.h"

#include <string_view>
#include <filesystem>
#include <fstream>
#include <utility>
#include <tuple>


auto _loadShaderFromFile(const std::string_view shaderPath) -> std::unique_ptr<char[]>;


void ResourceManager::LoadShaderProgram(const std::string& name, const char* vertPath, const char* fragPath, const char* geomPath /*= nullptr*/)
{
    const auto vertSource = _loadShaderFromFile(vertPath);
    const auto fragSource = _loadShaderFromFile(fragPath);
    const auto geomSource = geomPath != nullptr ? _loadShaderFromFile(geomPath) : nullptr;
    
    //m_shaderPrograms.insert({ name, GLShaderProgram(vertSource.get(), fragSource.get(), geomSource.get()) });
    m_shaderPrograms.emplace(std::piecewise_construct,
                             std::forward_as_tuple(name),
                             std::forward_as_tuple(vertSource.get(), fragSource.get(), geomSource.get()));
}

// NOTE: Name should be extracted from file name
void ResourceManager::LoadTexture(const std::string& name, bool alpha, const char* texturePath)
{
    i32 width, height, channels;
    ui8* data = stbi_load(texturePath, &width, &height, &channels, 0);

    m_textures.emplace(std::piecewise_construct,
                       std::forward_as_tuple(name),
                       std::forward_as_tuple(width, height, alpha, data));

    stbi_image_free(data);
}

const GLShaderProgram& ResourceManager::GetShaderProgram(const std::string& name)
{
    return m_shaderPrograms[name];
}

const GLTexture& ResourceManager::GetTexture(const std::string& name)
{
    return m_textures[name];
}


std::unique_ptr<char[]> _loadShaderFromFile(const std::string_view shaderPath)
{
    const auto size = std::filesystem::file_size(shaderPath);
    auto buffer = std::make_unique<char[]>(size + 1);

    std::ifstream shaderFile(shaderPath, std::ios::binary | std::ios::in);
    shaderFile.read(buffer.get(), size);

    return buffer;
}

//ui8* _loadTextureFromFile(const char* filePath)
//{
//    i32 width, height, channels;
//    ui8* data = stbi_load(filePath, &width, &height, &channels, 0);
//}
