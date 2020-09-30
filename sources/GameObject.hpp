#pragma once

#include "core.hpp"
#include "Graphics/OpenGL/GLTexture.hpp"
#include "SpriteRenderer.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>


class GameObject
{
public:
    glm::vec2 m_position, m_size, m_velocity;
    glm::vec3 m_color;
    f32 m_rotation;
    bool m_isSolid;
    bool m_isDestroyed = false;

public:
    GameObject() = default;
    GameObject(const GLTexture& texture, bool isSolid,
               glm::vec2 position, glm::vec2 size,
               glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f));

    virtual void Draw(const SpriteRenderer& renderer) const;

private:
    GLTexture m_texture;
};
