#include "core.hpp"
#include "GameObject.hpp"

#include <glm/vec2.hpp>


class BallObject : public GameObject
{
public:
    f32 m_radius;
    bool m_isStuck = true;
    bool m_isSticky = false;
    bool m_passThrough = false;

    //BallObject();
    BallObject(const GLTexture& texture, glm::vec2 position, f32 radius, glm::vec2 velocity);

    glm::vec2 Move(f32 dt, ui32 windowWidth);
    void Reset(glm::vec2 position, glm::vec2 velocity);
};
