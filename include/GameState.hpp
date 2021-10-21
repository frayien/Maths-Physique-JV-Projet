#ifndef MPJVP_GAMESTATE
#define MPJVP_GAMESTATE

#include <vector>
#include <memory>
#include <unordered_map>
#include "physics/Particle.hpp"
#include "physics/ParticleForceGenerator.hpp"

class GameState
{

public:
    GameState();
	~GameState();

    void addParticle(std::string label, std::unique_ptr<Particle> & particle);
    void addParticleForceGenerator(std::string label, std::unique_ptr<ParticleForceGenerator> & particleForceGenerator);

    inline std::unordered_map<std::string, std::unique_ptr<Particle>> & getParticles() { return m_particles; }
	inline std::unordered_map<std::string, std::unique_ptr<ParticleForceGenerator>> & getParticleForceGenerators() { return m_particleForceGenerators; }
    std::unique_ptr<Particle> & getParticle(std::string label);
    std::unique_ptr<ParticleForceGenerator> & getParticleForceGenerator(std::string label);

private:
    std::unordered_map<std::string, std::unique_ptr<Particle>> m_particles;
    std::unordered_map<std::string, std::unique_ptr<ParticleForceGenerator>> m_particleForceGenerators;
};

#endif // MPJVP_GAMESTATE
