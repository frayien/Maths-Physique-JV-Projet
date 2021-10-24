#ifndef MPJVP_PARTICLEBUOYANCY
#define MPJVP_PARTICLEBUOYANCY

#include "physics/forces/ParticleForceGenerator.hpp"

class ParticleBuoyancy : public ParticleForceGenerator 
{
private:
	float m_maxDepth;
    float m_volume;
	float m_waterHeight;
	float m_liquidDensity;

public:
	ParticleBuoyancy(float maxDepth, float volume, float waterHeight, float liquidDensity);
	virtual ~ParticleBuoyancy() = default;

	inline void setMaxDepth(float maxDepth) { m_maxDepth = maxDepth; }
	inline void setVolume(float volume) { m_volume = volume; }
	inline void setWaterHeight(float waterHeight) { m_waterHeight = waterHeight; }
	inline void setLiquidDensity(float liquidDensity) { m_liquidDensity = liquidDensity; }

	void updateForce(Particle* particle, float duration);
};

#endif // MPJVP_PARTICLEBUOYANCY