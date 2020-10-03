#include "GameLevel.hpp"

#include "ResourceManager.hpp"

#include <fstream>
#include <sstream>


GameLevel::GameLevel(const char* levelPath, ui32 levelWidth, ui32 levelHeight)
{
    std::ifstream levelFile(levelPath);

    std::vector<std::vector<ui32>> levelData;
    std::string line;

    while (std::getline(levelFile, line)) {
        std::istringstream buffer(line);

        levelData.push_back({ std::istream_iterator<ui32>(buffer),
                              std::istream_iterator<ui32>() });
    }

    levelDataToGameObjects(levelData, levelWidth, levelHeight);
}

void GameLevel::Draw(/*const SpriteRenderer& renderer*/) const
{
    for (const auto& brick : m_bricks) {
        if (brick.m_isDestroyed == false) {
            brick.Draw();
        }
    }
}

bool GameLevel::IsCompleted() const
{
    for (const auto& brick : m_bricks) {
        if (brick.m_isSolid == false && brick.m_isDestroyed == false) {
            return false;
        }
    }

    return true;
}

void GameLevel::Reset()
{
    for (auto& brick : m_bricks) {
        if (brick.m_isSolid == false && brick.m_isDestroyed == true) {
            brick.m_isDestroyed = false;
        }
    }
}


void GameLevel::levelDataToGameObjects(const std::vector<std::vector<ui32>>& levelData, ui32 levelWidth, ui32 levelHeight)
{
    const auto height = levelData.size();
    const auto width = levelData[0].size();
    const f32 unitWidth = static_cast<f32>(levelWidth) / width;
    const f32 unitHeight = static_cast<f32>(levelHeight) / height;

    const auto& blockSolidTexture = ResourceManager::GetTexture("block_solid");
    const auto& blockTexture = ResourceManager::GetTexture("block");
    const glm::vec2 size(unitWidth, unitHeight);

    for (ui32 y = 0; y < height; ++y) {
        for (ui32 x = 0; x < width; ++x) {
            const glm::vec2 position(unitWidth * x, unitHeight * y);
            const auto block = levelData.at(y).at(x);// [y] [x] ;

            if (block == 1) {
                constexpr auto color = glm::vec3(0.8f, 0.8f, 0.7f);
                m_bricks.emplace_back(blockSolidTexture, true, position, size, color);
                //
            } else if (block > 1) {
                auto color = glm::vec3(1.0f);
                switch (block) {
                    case 2: color = glm::vec3(0.2f, 0.6f, 1.0f); break;
                    case 3: color = glm::vec3(0.0f, 0.7f, 0.0f); break;
                    case 4: color = glm::vec3(0.8f, 0.8f, 0.4f); break;
                    case 5: color = glm::vec3(1.0f, 0.5f, 0.0f); break;
                }
                m_bricks.emplace_back(blockTexture, false, position, size, color);
            }
        }
    }
}
