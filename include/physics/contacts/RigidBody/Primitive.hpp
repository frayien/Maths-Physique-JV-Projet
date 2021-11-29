#ifndef MPJVP_PRIMITIVE
#define MPJVP_PRIMITIVE

#include "physics/RigidBody.hpp"
#include "physics/Matrix34.hpp"
#include "physics/Vector3f.hpp"

class Primitive 
{
public:
	RigidBody* rigidBody;
	Matrix34 offset;

	inline Vector3f getPosition()
	{
		return offset * rigidBody->getPosition();
	}
};

#endif // MPJVP_PRIMITIVE