#include "GameState.hpp"

GameState::GameState()
{

}

GameState::~GameState()
{

}

bool GameState::addParticle(const std::string & label, std::unique_ptr<Particle> && particle)
{
    return m_particles.insert({label, std::move(particle)}).second;
}

bool GameState::addParticleForceGenerator(const std::string & label, std::unique_ptr<ParticleForceGenerator> && particleForceGenerator)
{
    return m_particleForceGenerators.insert({label, std::move(particleForceGenerator)}).second;
}

bool GameState::addShapeGenerator(const std::string & label, std::unique_ptr<IShapeGenerator> && shapeGenerator)
{
    return m_shapeGenerators.insert({label, std::move(shapeGenerator)}).second;
}

bool GameState::removeShapeGenerator(const std::string & label)
{
    auto pos = m_shapeGenerators.find(label);
    if(pos == m_shapeGenerators.end()) return false;

    m_shapeGenerators.erase(pos);

    return true;
}