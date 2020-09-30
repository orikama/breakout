#pragma once

#include "core.hpp"

#include "Graphics/OpenGL/GLTexture.hpp"
#include "GameObject.hpp"

#include <glm/vec3.hpp>


class PowerUp : public GameObject
{
public:
    enum class Type
    {
        Speed, Sticky, PassThrough, PadSizeIncrease, Confuse, Chaos
    };

    Type m_type;
    f32 m_duration;
    bool m_isActive = false;


    PowerUp(Type type, const GLTexture& texture, f32 duration, glm::vec2 position, glm::vec3 color);
};
