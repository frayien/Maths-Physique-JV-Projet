#include "physics/forces/RigidBody/RigidBodyForceRegistry.hpp"

RigidBodyForceRegistry::RigidBodyForceRegistry()
{
}

RigidBodyForceRegistry::~RigidBodyForceRegistry()
{
}

void RigidBodyForceRegistry::addForce(RigidBody* rigidBody, RigidBodyForceGenerator* rigidBodyForceGenerator, float duration)
{
	RigidBodyForceEntry newEntry;

	newEntry.rigidBody = rigidBody;
	newEntry.rigidBodyForceGenerator = rigidBodyForceGenerator;

	m_registry.push_back(newEntry);
}

void RigidBodyForceRegistry::removeForce(RigidBodyForceGenerator* _rigidBodyForceGenerator)
{
	for (std::vector<RigidBodyForceEntry>::iterator it = m_registry.begin(); it != m_registry.end(); ++it) {
		if (it->rigidBodyForceGenerator == _rigidBodyForceGenerator) {
			m_registry.erase(it);
		}
	}
}

void RigidBodyForceRegistry::update(float deltaTime)
{
	for (auto& [rigidBody, rigidBodyForceGenerator] : m_registry)
	{
		rigidBodyForceGenerator->updateForce(rigidBody, deltaTime);
	}
}