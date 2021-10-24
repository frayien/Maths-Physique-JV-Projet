#ifndef MPJVP_LINKSHAPEGENERATOR
#define MPJVP_LINKSHAPEGENERATOR

#include "render/shape/IShapeGenerator.hpp"

#include "render/shape/CubeShapeGenerator.hpp"
#include "physics/Particle.hpp"

class LinkShapeGenerator : public IShapeGenerator
{
private:
    Particle* m_particleA;
    Particle* m_particleB;
    CubeShapeGenerator m_cube;
    
public:
    LinkShapeGenerator(Particle* particleA, Particle* particleB, color_t color = Color::GRAY);
    virtual ~LinkShapeGenerator();

    virtual void addShape(std::vector<Shape> & shapes) override;
};


#endif // MPJVP_LINKSHAPEGENERATOR
