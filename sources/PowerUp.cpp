#include "PowerUp.hpp"


constexpr auto kPowerUpSize = glm::vec2(60.0f, 20.0f);
constexpr auto kPowerUpVelocity = glm::vec2(0.0f, 150.0f);


PowerUp::PowerUp(Type type, const GLTexture& texture, f32 duration, glm::vec2 position, glm::vec3 color)
    : GameObject(texture, false, position, kPowerUpSize, color, kPowerUpVelocity)
    , m_type(type)
    , m_duration(duration)
{}
