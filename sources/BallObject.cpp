#include "BallObject.hpp"


//BallObject::BallObject()
//    : GameObject()
//    , m_radius(12.5f)
//{}

BallObject::BallObject(const GLTexture& texture, glm::vec2 position, f32 radius, glm::vec2 velocity)
    : GameObject(texture, false, position, glm::vec2(radius * 2.0f), glm::vec4(1.0f), velocity)
    , m_radius(radius)
{}

glm::vec2 BallObject::Move(f32 dt, ui32 windowWidth)
{
    if (m_isStuck == false) {
        m_position += m_velocity * dt;

        if (m_position.x <= 0.0f) {
            m_velocity.x = -m_velocity.x;
            m_position.x = 0.0f;
        } else if (m_position.x + m_size.x >= windowWidth) {
            m_velocity.x = -m_velocity.x;
            m_position.x = windowWidth - m_size.x;
        }

        if (m_position.y <= 0.0f) {
            m_velocity.y = -m_velocity.y;
            m_position.y = 0.0f;
        }
    }

    return m_position;
}

void BallObject::Reset(glm::vec2 position, glm::vec2 velocity)
{
    m_position = position;
    m_velocity = velocity;
    m_isStuck = true;
}
