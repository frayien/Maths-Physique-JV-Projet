#ifndef MPJVP_RIGIDBODYGRAVITY
#define MPJVP_RIGIDBODYGRAVITY

#include "physics/forces/RigidBody/RigidBodyForceGenerator.hpp"
#include "physics/Vector3f.hpp"

class RigidBodyGravity : public RigidBodyForceGenerator
{
private:
	Vector3f m_gravity;
public:
	RigidBodyGravity();
	RigidBodyGravity(float gravityValue);
	virtual ~RigidBodyGravity();

	void setGravity(float value);

	void updateForce(RigidBody* rigidBody, float duration);

};

#endif // MPJVP_RIGIDBODYGRAVITY