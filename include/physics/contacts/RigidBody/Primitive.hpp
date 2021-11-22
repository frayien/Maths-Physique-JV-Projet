#ifndef MPJVP_PRIMITIVE
#define MPJVP_PRIMITIVE

#include "physics/RigidBody.hpp"
#include "physics/Matrix34.hpp"

class Primitive 
{
public:
	RigidBody* rigidBody;
	Matrix34 offset;

	virtual void Primitive() = 0;
	virtual ~Primitive() = default;
};

#endif // MPJVP_PRIMITIVE