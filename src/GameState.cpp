#include "GameState.hpp"

GameState::GameState()
{

}

GameState::~GameState()
{

}

void GameState::addParticle(std::string label, std::unique_ptr<Particle> & particle)
{
    m_particles.insert({label, std::move(particle)});
}

void GameState::addParticleForceGenerator(std::string label, std::unique_ptr<ParticleForceGenerator> & particleForceGenerator)
{
    m_particleForceGenerators.insert({label, std::move(particleForceGenerator)});
}

std::unique_ptr<Particle> & GameState::getParticle(std::string label)
{
    return m_particles.find(label)->second;
    // TODO: handle particle not found
}

std::unique_ptr<ParticleForceGenerator> & GameState::getParticleForceGenerator(std::string label)
{
    return m_particleForceGenerators.find(label)->second;
    // TODO: handle force not found
}