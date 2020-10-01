#pragma once

#include "core.hpp"
#include "Graphics/OpenGL/GLShaderProgram.hpp"
#include "Graphics/OpenGL/GLTexture.hpp"
#include "GameObject.hpp"

#include <vector>

// TODO: Amount of particles should not depend on frame rate.
//  Probably just expand vector if it hits the limit.
//  Or it would be better to track how many particles to spawn in the current frame.
//  No need to spawn thousands of them every frame
class ParticleGenerator
{
public:
    ParticleGenerator(const GLShaderProgram& shaderProgram, const GLTexture& texture, ui32 amountOfParticles);

    void Update(f32 dt, const GameObject& obj, ui32 newParticles, glm::vec2 offset = { 0.0f, 0.0f });
    void Draw() const;

private:
    struct Particle
    {
        glm::vec2 position;
        glm::vec2 velocity;
        glm::vec4 color;
        f32 life;

        Particle() : position(), velocity(), color(1.0f), life() {}
    };


    void initVAO();
    ui32 firstUnusedParticle() const;
    void respawnParticle(Particle& particle, const GameObject& obj, glm::vec2 offset);


    const GLShaderProgram& m_shaderProgram;
    const GLTexture& m_texture;
    ui32 m_quadVAO;

    std::vector<Particle> m_particles;
    ui32 m_amountOfParticles;
};
