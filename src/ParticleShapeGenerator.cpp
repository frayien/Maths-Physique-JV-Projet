#include "ParticleShapeGenerator.hpp"

ParticleShapeGenerator::ParticleShapeGenerator(Particle* particle, color_t color) :
    m_sphere{color},
    m_particle{particle}
{
    m_sphere.scale(0.2f);
    //m_sphere.scale(particle->getRadius());
}

ParticleShapeGenerator::~ParticleShapeGenerator()
{
}

void ParticleShapeGenerator::addShape(std::vector<Shape> & shapes)
{
    m_sphere.setPosition(m_particle->getPosition());
    m_sphere.addShape(shapes);
}