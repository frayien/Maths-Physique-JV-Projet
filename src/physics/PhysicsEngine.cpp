#include "physics/PhysicsEngine.hpp"

#include <vector>

PhysicsEngine::PhysicsEngine()
{

}

PhysicsEngine::~PhysicsEngine()
{

}

void PhysicsEngine::update(float deltaTime, std::vector<Particle *> & particles)
{
    m_particleRegistry.update(deltaTime);

    for (Particle * p : particles)
    {
        p->integrate(deltaTime);
    }
}