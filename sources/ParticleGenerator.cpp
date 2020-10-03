#include "ParticleGenerator.hpp"

#include "Graphics/Renderer2D.hpp"

#include <cstdlib>


ParticleGenerator::ParticleGenerator(const GLTexture& texture, ui32 amountOfParticles)
    : m_texture(texture)
    , m_amountOfParticles(amountOfParticles)
{
    m_particles.resize(amountOfParticles);
}


void ParticleGenerator::Update(f32 dt, const GameObject& obj, ui32 newParticles, glm::vec2 offset /*= { 0.0f, 0.0f }*/)
{
    for (ui32 i = 0; i < newParticles; ++i) {
        auto unusedParticle = firstUnusedParticle();
        respawnParticle(m_particles[unusedParticle], obj, offset);
    }

    for (auto& particle : m_particles) {
        // NOTE: 'life' can go below zero, how far depends on a lot of things, idk if it's affects anything
        particle.life -= dt;
        if (particle.life > 0.0f) {
            particle.position -= particle.velocity * dt;
            particle.color.a -= dt * 2.5f;
        }
    }
}

void ParticleGenerator::Draw() const
{
    Renderer2D::SetBlendFunction(Renderer2D::BlendFactor::SrcAlpha, Renderer2D::BlendFactor::One);

    for (const auto& particle : m_particles) {
        if (particle.life > 0.0f) {
            Renderer2D::DrawQuad(m_texture, particle.position, glm::vec2(10.0f), 0.0f, particle.color);
        }
    }

    // NOTE: I guess technically I should first backup old glBlendFunc state (through glGet()),
    //  set new one and at the end reset it to previous one
    Renderer2D::SetBlendFunction(Renderer2D::BlendFactor::SrcAlpha, Renderer2D::BlendFactor::OneMinusSrcAlpha);
}


// NOTE: Yeah, I'm sure there is no better way of tracking particles ;)
ui32 lastUsedParticle = 0;
ui32 ParticleGenerator::firstUnusedParticle() const
{
    // NOTE: shouldn't it be 'i < m_particles.size()' ?
    for (ui32 i = lastUsedParticle; i < m_amountOfParticles; ++i) {
        if (m_particles[i].life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }

    for (ui32 i = 0; i < lastUsedParticle; ++i) {
        if (m_particles[i].life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }

    printf("LOOOL\n");
    lastUsedParticle = 0;
    return 0;
}

void ParticleGenerator::respawnParticle(Particle& particle, const GameObject& obj, glm::vec2 offset)
{
    // NOTE: rand() in the current year, smh
    auto rPosition = (std::rand() % 100 - 50) / 10.0f;
    auto rColor = 0.5f + (std::rand() % 100) / 100.0f;

    particle.position = obj.m_position + rPosition + offset;
    particle.color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.velocity = obj.m_velocity * 0.1f;
    particle.life = 1.0f;
}
