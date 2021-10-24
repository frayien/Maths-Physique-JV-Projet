#ifndef MPJVP_PARTICLESHAPEGENERATOR
#define MPJVP_PARTICLESHAPEGENERATOR

#include "render/shape/IShapeGenerator.hpp"

#include "render/shape/SphereShapeGenerator.hpp"
#include "physics/Particle.hpp"

class ParticleShapeGenerator : public IShapeGenerator
{
private:
    SphereShapeGenerator m_sphere;
    Particle* m_particle;
    
public:
    ParticleShapeGenerator(Particle* particle, color_t color = Color::GRAY);
    virtual ~ParticleShapeGenerator();

    virtual void addShape(std::vector<Shape> & shapes) override;

    inline SphereShapeGenerator & getSphere() { return m_sphere; }
};

#endif // MPJVP_PARTICLESHAPEGENERATOR
