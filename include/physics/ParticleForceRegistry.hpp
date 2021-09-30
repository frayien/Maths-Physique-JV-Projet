#ifndef MPJVP_PARTICLEFORCEREGISTRY
#define MPJVP_PARTICLEFORCEREGISTRY

#include "physics/ParticleForceGenerator.hpp"

class ParticleForceRegistry 
{
public:
	struct ParticleForceEntry
	{
		Particle* particle;
		ParticleForceGenerator* particleForceGenerator;
	};
	typedef ParticleForceEntry Entry;

	using Registry = std::vector<Entry>;

	ParticleForceRegistry();
	~ParticleForceRegistry();
	inline Registry getRegistry() { return m_registry; }
	void addForce(Particle* particle, ParticleForceGenerator* particleForceGenerator, float duration);
	void removeForce(ParticleForceGenerator* _particleForceGenerator);

private:
	Registry m_registry;

};

#endif // MPJVP_PARTICLEFORCEREGISTRY
