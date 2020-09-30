#include "GameObject.hpp"


GameObject::GameObject(const GLTexture& texture, bool isSolid,
                       glm::vec2 position, glm::vec2 size,
                       glm::vec3 color /*= glm::vec3(1.0f)*/, glm::vec2 velocity /*= glm::vec2(0.0f)*/)
    : m_texture(texture)
    , m_position(position)
    , m_size(size)
    , m_velocity(velocity)
    , m_color(color)
    , m_rotation(0.0f)
    , m_isSolid(isSolid)
{}


void GameObject::Draw(const SpriteRenderer& renderer) const
{
    renderer.DrawSprite(m_texture, m_position, m_size, m_rotation, m_color);
}
