#pragma once

#include "core.hpp"
#include "SpriteRenderer.hpp"
#include "ParticleGenerator.hpp"
#include "PostProcessor.hpp"
#include "GameLevel.hpp"
#include "BallObject.hpp"
#include "PowerUp.hpp"

#include <memory>


class Game
{
public:
    enum class GameState : ui8
    {
        Active, Menu, Win
    };

    bool m_keys[1024] = {};

    Game(ui32 width, ui32 height);
    ~Game() = default;

    //void Init();

    void ProcessInput(f32 dt);
    void Update(f32 dt);
    void Render(f32 time);

    void DoCollisions();

    void ResetPlayerAndBall();

    void SpawnPowerUps(const GameObject& brick);
    void UpdatePowerUps(f32 dt);

private:
    void activatePowerUp(const PowerUp& powerUp);
    bool isOtherPowerUpActive(PowerUp::Type type) const;


    ui32 m_width;
    ui32 m_height;
    GameState m_gameState;
    f32 m_shakeTime = 0.0f;

    std::unique_ptr<SpriteRenderer> m_renderer;
    std::unique_ptr<ParticleGenerator> m_particleGenerator;
    std::unique_ptr<PostProcessor> m_postProcessor;

    std::unique_ptr<GameObject> m_player;
    std::unique_ptr<BallObject> m_ball;

    std::vector<GameLevel> m_levels;
    ui32 m_currentLevel = 0;

    std::vector<PowerUp> m_powerUps;
};
