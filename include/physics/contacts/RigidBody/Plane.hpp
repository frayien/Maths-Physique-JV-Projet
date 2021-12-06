#ifndef MPJVP_PLANE
#define MPJVP_PLANE

#include "physics/contacts/RigidBody/Primitive.hpp"
#include "physics/Vector3f.hpp"

class Plane : public Primitive
{
public:
	Vector3f normal;
	float thickness; // probablement l'épaisseur du plan

	inline Vector3f getNormal() { return normal; }
	inline float getThickness() { return thickness; }
};

#endif // MPJVP_PLANE