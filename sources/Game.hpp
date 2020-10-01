#pragma once

#include "core.hpp"
#include "SpriteRenderer.hpp"
#include "ParticleGenerator.hpp"
#include "PostProcessor.hpp"
#include "TextRenderer.hpp"
#include "GameLevel.hpp"
#include "BallObject.hpp"
#include "PowerUp.hpp"

#include <memory>


class Game
{
public:
    bool m_keys[300] = {};
    bool m_keysProcessed[300] = {};


    Game(ui32 width, ui32 height);
    ~Game() = default;

    void ProcessInput(f32 dt);
    void Update(f32 dt);
    void Render(f32 time);

private:
    enum class GameState : ui8
    {
        Active, Menu, Win
    };


    void DoCollisions();

    void resetPlayerAndBall();

    void activatePowerUp(const PowerUp& powerUp);
    bool isOtherPowerUpActive(PowerUp::Type type) const;
    void spawnPowerUps(const GameObject& brick);
    void updatePowerUps(f32 dt);


    std::unique_ptr<SpriteRenderer> m_renderer;
    std::unique_ptr<ParticleGenerator> m_particleGenerator;
    std::unique_ptr<PostProcessor> m_postProcessor;
    std::unique_ptr<TextRenderer> m_textRenderer;

    std::unique_ptr<GameObject> m_player;
    std::unique_ptr<BallObject> m_ball;

    std::vector<GameLevel> m_levels;
    i32 m_currentLevel = 0;

    std::vector<PowerUp> m_powerUps;

    ui32 m_width;
    ui32 m_height;
    GameState m_gameState;
    f32 m_shakeTime = 0.0f;

    ui32 m_playerLives;
};
