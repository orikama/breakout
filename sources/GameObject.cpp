#include "GameObject.hpp"

#include "Graphics/Renderer2D.hpp"


GameObject::GameObject(const GLTexture& texture, bool isSolid,
                       glm::vec2 position, glm::vec2 size,
                       glm::vec4 color /*= glm::vec3(1.0f)*/, glm::vec2 velocity /*= glm::vec2(0.0f)*/)
    : m_texture(texture)
    , m_position(position)
    , m_size(size)
    , m_velocity(velocity)
    , m_color(color)
    , m_rotation(0.0f)
    , m_isSolid(isSolid)
{}


void GameObject::Draw() const
{
    Renderer2D::DrawQuad(m_texture, m_position, m_size, m_rotation, m_color);
}
