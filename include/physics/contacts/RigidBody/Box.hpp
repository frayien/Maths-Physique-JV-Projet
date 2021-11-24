#ifndef MPJVP_BOX
#define MPJVP_BOX

#include "physics/contacts/RigidBody/Primitive.hpp"
#include "physics/Vector3f.hpp"

class Box : public Primitive
{
public:
	Vector3f halfSizeX;
	Vector3f halfSizeY;
};

#endif // MPJVP_BOX