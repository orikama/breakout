#pragma once

#include "core.hpp"
#include "SpriteRenderer.hpp"
#include "GameObject.hpp"

#include <vector>


class GameLevel
{
public:
    std::vector<GameObject> m_bricks;

public:
    GameLevel(const char* levelPath, ui32 levelWidth, ui32 levelHeight);

    void Draw(const SpriteRenderer& renderer) const;

    bool IsCompleted() const;

    void Reset();

private:
    void levelDataToGameObjects(const std::vector<std::vector<ui32>>& levelData, ui32 levelWidth, ui32 levelHeight);
};
