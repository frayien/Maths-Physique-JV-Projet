#ifndef MPJVP_SPHERE
#define MPJVP_SPHERE

#include "physics/contacts/RigidBody/Primitive.hpp"

class Sphere : public Primitive 
{
public:
	Sphere(RigidBody* rigidBody, Matrix34 offset, float radius);
	~Sphere() = default;

	inline float getRadius() const { return m_radius; }

private:
	float m_radius;
};

#endif // MPJVP_SPHERE