#include "Game.hpp"
#include "ResourceManager.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <optional>
#include <utility>
#include <cmath>
#include <cstdlib>


constexpr auto kPlayerSize = glm::vec2(100.0f, 20.0f);
constexpr auto kPlayerVelocity = 500.0f;

constexpr auto kBallRadius = 12.5f;
constexpr auto kBallInitialVelocity = glm::vec2(100.0f, -350.0f);


enum class Direction
{
    Up, Right, Down, Left
};

using Collision = std::optional<std::pair<Direction, glm::vec2>>;


bool checkCollision(const GameObject& one, const GameObject& two);
Collision checkCollision(const BallObject& ball, const GameObject& obj);
Direction getCollisionDirection(glm::vec2 target);

bool shouldSpawn(ui32 chance);


Game::Game(ui32 width, ui32 height)
    : m_width(width)
    , m_height(height)
    , m_gameState(GameState::Active)
{
    // SHADERS
    auto projection = glm::ortho<f32>(0.0f, width, height, 0.0f, -1.0f, 1.0f);

    ResourceManager::LoadShaderProgram("sprite", "shaders/sprite.vert", "shaders/sprite.frag");
    ResourceManager::LoadShaderProgram("particle", "shaders/particle.vert", "shaders/particle.frag");
    ResourceManager::LoadShaderProgram("postprocess", "shaders/postprocess.vert", "shaders/postprocess.frag");

    const auto& spriteShader = ResourceManager::GetShaderProgram("sprite");
    spriteShader.Bind();
    spriteShader.SetMatrix4("u_projection", projection);
    spriteShader.SetInt1("u_texture", 0);
    const auto& particleShader = ResourceManager::GetShaderProgram("particle");
    particleShader.Bind();
    particleShader.SetMatrix4("u_projection", projection);
    particleShader.SetInt1("u_texture", 0);

    // TEXTURES
    ResourceManager::LoadTexture("background", false, "textures/background.jpg");
    ResourceManager::LoadTexture("block", false, "textures/block.png");
    ResourceManager::LoadTexture("block_solid", false, "textures/block_solid.png");
    ResourceManager::LoadTexture("paddle", true, "textures/paddle.png");
    ResourceManager::LoadTexture("ball", true, "textures/awesomeface.png");
    ResourceManager::LoadTexture("particle", true, "textures/particle.png");

    ResourceManager::LoadTexture("powerup_speed", true, "textures/powerup_speed.png");
    ResourceManager::LoadTexture("powerup_sticky", true, "textures/powerup_sticky.png");
    ResourceManager::LoadTexture("powerup_passthrough", true, "textures/powerup_passthrough.png");
    ResourceManager::LoadTexture("powerup_increase", true, "textures/powerup_increase.png");
    ResourceManager::LoadTexture("powerup_confuse", true, "textures/powerup_confuse.png");
    ResourceManager::LoadTexture("powerup_chaos", true, "textures/powerup_chaos.png");

    // LEVELS
    m_levels.emplace_back("levels/1.lvl", width, height / 2);
    m_levels.emplace_back("levels/2.lvl", width, height / 2);
    m_levels.emplace_back("levels/3.lvl", width, height / 2);
    m_levels.emplace_back("levels/4.lvl", width, height / 2);

    // SYSTEMS
    m_renderer = std::make_unique<SpriteRenderer>(spriteShader);
    m_particleGenerator = std::make_unique<ParticleGenerator>(particleShader, ResourceManager::GetTexture("particle"), 1000.0f);
    m_postProcessor = std::make_unique<PostProcessor>(ResourceManager::GetShaderProgram("postprocess"), width, height);

    // GAME OBJECTS
    const auto playerPosition = glm::vec2((width - kPlayerSize.x) / 2.0f, height - kPlayerSize.y);
    m_player = std::make_unique<GameObject>(ResourceManager::GetTexture("paddle"), false, playerPosition, kPlayerSize);

    const auto ballPosition = playerPosition + glm::vec2(kPlayerSize.x / 2.0f - kBallRadius, -kBallRadius * 2.0f);
    m_ball = std::make_unique<BallObject>(ResourceManager::GetTexture("ball"), ballPosition, kBallRadius, kBallInitialVelocity);
}

//void Init();

void Game::ProcessInput(f32 dt)
{
    if (m_gameState == GameState::Active) {
        auto velocity = kPlayerVelocity * dt;

        // TODO: Remove hardcoded key codes
        if (m_keys[65]) {   // 'A'
            // FIXME: WTF IS THIS, why we not take in to account player size, or velocity.
            //  Player can get go off-scrren
            if (m_player->m_position.x >= 0.0f) {
                m_player->m_position.x -= velocity;
                if (m_ball->m_isStuck) {
                    m_ball->m_position.x -= velocity;
                }
            }
        }
        if (m_keys[68]) {   // 'D'
            if (m_player->m_position.x <= m_width - m_player->m_size.x) {
                m_player->m_position.x += velocity;
                if (m_ball->m_isStuck) {
                    m_ball->m_position.x += velocity;
                }
            }
        }

        if (m_keys[32]) {   // SPACE
            m_ball->m_isStuck = false;
        }
    }
}

void Game::Update(f32 dt)
{
    m_ball->Move(dt, m_width);

    DoCollisions();

    m_particleGenerator->Update(dt, *m_ball, 2, glm::vec2(m_ball->m_radius / 2.0f));

    UpdatePowerUps(dt);

    if (m_shakeTime > 0.0f) {
        m_shakeTime -= dt;
        if (m_shakeTime <= 0.0f) {
            m_postProcessor->m_isShake = false;
        }
    }

    // did ball reach bottom edge?
    if (m_ball->m_position.y >= m_height) {
        ResetPlayerAndBall();
        m_levels[m_currentLevel].Reset();
    }
}

void Game::Render(f32 time)
{
    if (m_gameState == GameState::Active) {
        m_postProcessor->BeginRender();
        {
            m_renderer->DrawSprite(ResourceManager::GetTexture("background"),
                                   glm::vec2(0.0f), glm::vec2(m_width, m_height));
            m_levels[m_currentLevel].Draw(*m_renderer);
            m_player->Draw(*m_renderer);

            for (const auto& powerUp : m_powerUps) {
                if (powerUp.m_isDestroyed == false) {
                    powerUp.Draw(*m_renderer);
                }
            }

            m_particleGenerator->Draw();
            m_ball->Draw(*m_renderer);
        }
        m_postProcessor->EndRender();
        m_postProcessor->Render(time);
    }
}


// NOTE: Use glm::reflect() ?
void Game::DoCollisions()
{
    // check collisions between bricks and ball
    for (auto& brick : m_levels[m_currentLevel].m_bricks) {
        if (brick.m_isDestroyed == false) {
            if (const auto collision = checkCollision(*m_ball, brick); collision.has_value()) {
                if (brick.m_isSolid == false) {
                    brick.m_isDestroyed = true;
                    SpawnPowerUps(brick);
                } else {
                    m_shakeTime = 0.5f;
                    m_postProcessor->m_isShake = true;
                }

                if (m_ball->m_passThrough == false || brick.m_isSolid) {
                    const auto& [direction, difference] = collision.value();

                    if (direction == Direction::Left || direction == Direction::Right) {
                        m_ball->m_velocity.x = -m_ball->m_velocity.x;
                        // relocate ball
                        auto penetraionDistance = m_ball->m_radius - std::abs(difference.x);
                        if (direction == Direction::Left) {
                            m_ball->m_position.x += penetraionDistance;
                        } else {
                            m_ball->m_position.x -= penetraionDistance;
                        }
                    } else {
                        m_ball->m_velocity.y = -m_ball->m_velocity.y;
                        // relocate ball
                        auto penetraionDistance = m_ball->m_radius - std::abs(difference.y);
                        if (direction == Direction::Down) {
                            m_ball->m_position.y += penetraionDistance;
                        } else {
                            m_ball->m_position.y -= penetraionDistance;
                        }
                    }
                }
            }
        }
    }
    // check collisions between powerUps and player pad
    for (auto& powerUp : m_powerUps) {
        if (powerUp.m_isDestroyed == false) {
            if (powerUp.m_position.y >= m_height) {
                powerUp.m_isDestroyed = true;
            } else if (checkCollision(*m_player, powerUp)) {
                checkCollision(*m_player, powerUp);
                powerUp.m_isDestroyed = true;
                powerUp.m_isActive = true;
                activatePowerUp(powerUp);
            }
        }
    }
    // check collision between ball and player pad
    if (m_ball->m_isStuck == false) {
        if (const auto collision = checkCollision(*m_ball, *m_player); collision.has_value()) {
            // check where is the ball hit the board
            auto boardCenter = m_player->m_position.x + m_player->m_size.x / 2.0f;
            auto distance = m_ball->m_position.x + m_ball->m_radius - boardCenter;
            auto percentage = distance / (m_player->m_size.x / 2.0f);
            // and change velocity accordingly
            constexpr auto strength = 2.0f;
            auto newVelocity = glm::vec2(kBallInitialVelocity.x * percentage * strength,
                                         -1.0f * std::abs(m_ball->m_velocity.y));
            m_ball->m_velocity = glm::normalize(newVelocity) * glm::length(m_ball->m_velocity);

            // if Sticky powerup is activated, also stick ball to paddle once new velocity vectors were calculated
            m_ball->m_isStuck = m_ball->m_isSticky;
        }
    }
}


void Game::ResetPlayerAndBall()
{
    m_player->m_size = kPlayerSize;
    m_player->m_position = glm::vec2((m_width - kPlayerSize.x) / 2.0f, m_height - kPlayerSize.y);

    const auto ballPosition = m_player->m_position + glm::vec2(kPlayerSize.x / 2.0f - kBallRadius, -kBallRadius * 2.0f);
    m_ball->Reset(ballPosition, kBallInitialVelocity);
}


void Game::SpawnPowerUps(const GameObject& block)
{
    // 1 in 75 chance
    if (shouldSpawn(75)) {
        m_powerUps.emplace_back(PowerUp::Type::Speed, ResourceManager::GetTexture("powerup_speed"),
                                0.0f, block.m_position, glm::vec3(0.5f, 0.5f, 1.0f));
    }
    if (shouldSpawn(75)) {
        m_powerUps.emplace_back(PowerUp::Type::Sticky, ResourceManager::GetTexture("powerup_sticky"),
                                20.0f, block.m_position, glm::vec3(1.0f, 0.5f, 1.0f));
    }
    if (shouldSpawn(75)) {
        m_powerUps.emplace_back(PowerUp::Type::PassThrough, ResourceManager::GetTexture("powerup_passthrough"),
                                10.0f, block.m_position, glm::vec3(0.5f, 1.0f, 0.5f));
    }
    if (shouldSpawn(75)) {
        m_powerUps.emplace_back(PowerUp::Type::PadSizeIncrease, ResourceManager::GetTexture("powerup_increase"),
                                0.0f, block.m_position, glm::vec3(1.0f, 0.6f, 0.4));
    }
    // Negative powerups should spawn more often
    if (shouldSpawn(15)) {
        m_powerUps.emplace_back(PowerUp::Type::Confuse, ResourceManager::GetTexture("powerup_confuse"),
                                15.0f, block.m_position, glm::vec3(1.0f, 0.3f, 0.3f));
    }
    if (shouldSpawn(15)) {
        m_powerUps.emplace_back(PowerUp::Type::Chaos, ResourceManager::GetTexture("powerup_chaos"),
                                15.0f, block.m_position, glm::vec3(0.9f, 0.25f, 0.25f));
    }
}

void Game::UpdatePowerUps(f32 dt)
{
    for (auto& powerUp : m_powerUps) {
        if (powerUp.m_isDestroyed == false) {
            powerUp.m_position += powerUp.m_velocity * dt;
        }
        if (powerUp.m_isActive) {
            powerUp.m_duration -= dt;

            if (powerUp.m_duration <= 0.0f) {
                powerUp.m_isActive = false;

                switch (powerUp.m_type) {
                    case PowerUp::Type::Sticky: {
                            if (isOtherPowerUpActive(PowerUp::Type::Sticky) == false) {
                                m_ball->m_isSticky = false;
                                m_player->m_color = glm::vec3(1.0f);
                            }
                            break;
                        }
                    case PowerUp::Type::PassThrough: {
                            if (isOtherPowerUpActive(PowerUp::Type::PassThrough) == false) {
                                m_ball->m_passThrough = false;
                                m_player->m_color = glm::vec3(1.0f);
                            }
                            break;
                        }
                    case PowerUp::Type::Confuse: {
                            if (isOtherPowerUpActive(PowerUp::Type::Confuse) == false) {
                                m_postProcessor->m_isConfuse = false;
                            }
                            break;
                        }
                    case PowerUp::Type::Chaos: {
                            if (isOtherPowerUpActive(PowerUp::Type::Chaos) == false) {
                                m_postProcessor->m_isChaos = false;
                            }
                            break;
                        }
                }
            }
        }
    }

    m_powerUps.erase(std::remove_if(m_powerUps.begin(), m_powerUps.end(),
                                    [](const PowerUp& powerUp) {
                                        return powerUp.m_isDestroyed && powerUp.m_isActive == false;
                                    }),
                     m_powerUps.end());
    /*for (auto powerUp = m_powerUps.begin(); powerUp != m_powerUps.end(); ) {
        if (powerUp->m_isDestroyed && powerUp->m_isActive == false) {
            powerUp = m_powerUps.erase(powerUp);
        } else {
            ++powerUp;
        }
    }*/
}

void Game::activatePowerUp(const PowerUp& powerUp)
{
    switch (powerUp.m_type) {
        case PowerUp::Type::Speed: {
                m_ball->m_velocity *= 1.2f;
                break;
            }
        case PowerUp::Type::Sticky: {
                m_ball->m_isSticky = true;
                m_player->m_color = glm::vec3(1.0f, 0.5f, 1.0f);
                break;
            }
        case PowerUp::Type::PassThrough: {
                m_ball->m_passThrough = true;
                m_player->m_color = glm::vec3(1.0f, 0.5f, 0.5f);
                break;
            }
        case PowerUp::Type::PadSizeIncrease: {
                m_player->m_size.x += 50.0f;
                break;
            }
        case PowerUp::Type::Confuse: {
                if (m_postProcessor->m_isChaos == false) {
                    m_postProcessor->m_isConfuse = true;
                }
                break;
            }
        case PowerUp::Type::Chaos: {
                if (m_postProcessor->m_isConfuse == false) {
                    m_postProcessor->m_isChaos = true;
                }
                break;
            }
    }
}

bool Game::isOtherPowerUpActive(PowerUp::Type type) const
{
    // NOTE: Can use <algorithm> there, but since this whole PowerUp thing is shit and need to be rewritten it doesn't matter
    for (const auto& powerUp : m_powerUps) {
        if (powerUp.m_isActive && powerUp.m_type == type) {
            return true;
        }
    }

    return false;
}


bool checkCollision(const GameObject& one, const GameObject& two)
{
    // NOTE: Why not like this?
    //glm::greaterThanEqual(o1.m_position + o1.m_size, o2.m_position) && glm::greaterThanEqual(o2.m_position + o2.m_size, o1.m_position);

    bool collisionX = one.m_position.x + one.m_size.x >= two.m_position.x
        && two.m_position.x + two.m_size.x >= one.m_position.x;
    bool collisionY = one.m_position.y + one.m_size.y >= two.m_position.y
        && two.m_position.y + two.m_size.y >= one.m_position.y;

    return collisionX && collisionY;
}

Collision checkCollision(const BallObject& ball, const GameObject& obj)
{
    const auto circleCenter = ball.m_position + ball.m_radius;
    const auto boxHalfExtent = obj.m_size / 2.0f;
    const auto boxCenter = obj.m_position + boxHalfExtent;

    const auto centerDifference = circleCenter - boxCenter;
    const auto clampedDifference = glm::clamp(centerDifference, -boxHalfExtent, boxHalfExtent);
    // closest point on the box edge to the circle
    const auto closest = boxCenter + clampedDifference;
    const auto difference = closest - circleCenter;

    if (glm::length(difference) <= ball.m_radius) {
        return { {getCollisionDirection(difference), difference} };
    }

    return std::nullopt;
}

Direction getCollisionDirection(glm::vec2 target)
{
    const glm::vec2 directions[] = {
        glm::vec2(0.0f, 1.0f),  // up
        glm::vec2(1.0f, 0.0f),  // right
        glm::vec2(0.0f, -1.0f), // down
        glm::vec2(-1.0f, 0.0f)  // left
    };
    // NOTE: Dot product minimum is -1, but idk if in this case it matters
    f32 max = 0.0f;
    ui32 bestMatch = -1;

    target = glm::normalize(target);

    for (ui32 i = 0; i < 4; ++i) {
        auto dotProduct = glm::dot(target, directions[i]);
        if (dotProduct > max) {
            max = dotProduct;
            bestMatch = i;
        }
    }

    return static_cast<Direction>(bestMatch);
}


bool shouldSpawn(ui32 chance)
{
    return (std::rand() % chance) == 0;
}
