#ifndef FRAYIEN_RIGIDBODYFORCEREGISTRY
#define FRAYIEN_RIGIDBODYFORCEREGISTRY

#include "physics/forces/RigidBodyForceGenerator.hpp"

class RigidBodyForceRegistry
{
public:
	struct RigidBodyForceEntry
	{
		RigidBody* rigidBody;
		RigidBodyForceGenerator* rigidBodyForceGenerator;
	};

	using Registry = std::vector<RigidBodyForceEntry>;

	RigidBodyForceRegistry();
	virtual ~RigidBodyForceRegistry();

	void addForce(RigidBody* rigidBody, RigidBodyForceGenerator* rigidBodyForceGenerator, float duration);
	void removeForce(RigidBodyForceGenerator* _particleForceGenerator);

	void update(float deltaTime);

	inline void clear()
	{
		m_registry.clear();
	}

private:
	Registry m_registry;

};

#endif // FRAYIEN_PARTICLEFORCEREGISTRY