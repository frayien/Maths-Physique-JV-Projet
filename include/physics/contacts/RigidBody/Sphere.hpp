#ifndef MPJVP_SPHERE
#define MPJVP_SPHERE

#include "physics/contacts/RigidBody/Primitive.hpp"

class Sphere : public Primitive 
{
public:
	float radius;

	inline float getRadius() const { return radius; }
};

#endif // MPJVP_SPHERE