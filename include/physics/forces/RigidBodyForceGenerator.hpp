#ifndef MPJVP_RIGIDBODYFORCEGENERATOR
#define MPJVP_RIGIDBODYFORCEGENERATOR

#include "physics/RigidBody.hpp"
#include "physics/contacts/ParticleContact.hpp"

class RigidBodyForceGenerator
{
public:
	RigidBodyForceGenerator() = default;
	virtual ~RigidBodyForceGenerator() = default;

	virtual void updateForce(RigidBody* rigidBody, float duration) = 0;
};

#endif // MPJVP_RIGIDBODYFORCEGENERATOR