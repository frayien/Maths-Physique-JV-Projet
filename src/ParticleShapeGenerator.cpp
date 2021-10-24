#include "ParticleShapeGenerator.hpp"

ParticleShapeGenerator::ParticleShapeGenerator(Particle* particle, color_t color) :
    m_sphere{color},
    m_particle{particle}
{
    m_sphere.setScale(particle->getRadius());
}

ParticleShapeGenerator::~ParticleShapeGenerator()
{
}

void ParticleShapeGenerator::addShape(std::vector<Shape> & shapes)
{
    m_sphere.setPosition(m_particle->getPosition());
    m_sphere.addShape(shapes);
}