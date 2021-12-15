#ifndef MPJVP_CONTACTSHAPEGENERATOR
#define MPJVP_CONTACTSHAPEGENERATOR

#include "render/shape/IShapeGenerator.hpp"

#include "render/shape/SphereShapeGenerator.hpp"
#include "render/shape/CubeShapeGenerator.hpp"

#include "physics/contacts/RigidBody/RigidBodyContact.hpp"

class ContactShapeGenerator : public IShapeGenerator
{
private:
    RigidBodyContact m_contact;

    SphereShapeGenerator m_sphere;
    CubeShapeGenerator m_cube;

    float m_scale = 0.1;

public:
    ContactShapeGenerator(RigidBodyContact contact, color_t color = Color::GRAY);
    virtual ~ContactShapeGenerator() = default;

    virtual void addShape(std::vector<Shape> & shapes) override;

    inline void setScale(float scale) { m_scale = scale; }
};


#endif // MPJVP_CONTACTSHAPEGENERATOR
