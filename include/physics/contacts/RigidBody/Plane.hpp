#ifndef MPJVP_PLANE
#define MPJVP_PLANE

#include "physics/contacts/RigidBody/Primitive.hpp"
#include "physics/Vector3f.hpp"

class Plane : public Primitive
{
public:
	Vector3f normal;
	float offset; // probablement l'épaisseur du plan
};

#endif // MPJVP_PLANE