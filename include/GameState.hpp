#ifndef MPJVP_GAMESTATE
#define MPJVP_GAMESTATE

#include <vector>
#include <memory>
#include <unordered_map>
#include "physics/Particle.hpp"
#include "physics/forces/ParticleForceGenerator.hpp"
#include "physics/contacts/ParticleContactGenerator.hpp"
#include "render/shape/IShapeGenerator.hpp"

class GameState
{

public:
    GameState();
	virtual ~GameState();

    bool addParticle(const std::string & label, std::unique_ptr<Particle> && particle);
    bool addParticleForceGenerator(const std::string & label, std::unique_ptr<ParticleForceGenerator> && particleForceGenerator);
    bool addParticleContactGenerator(const std::string & label, std::unique_ptr<ParticleContactGenerator> && particleContactGenerator);
    bool addShapeGenerator(const std::string & label, std::unique_ptr<IShapeGenerator> && shapeGenerator);

    bool removeShapeGenerator(const std::string & label);

    inline std::unordered_map<std::string, std::unique_ptr<Particle>> & getParticles() { return m_particles; }
	inline std::unordered_map<std::string, std::unique_ptr<ParticleForceGenerator>> & getParticleForceGenerators() { return m_particleForceGenerators; }
	inline std::unordered_map<std::string, std::unique_ptr<ParticleContactGenerator>> & getParticleContactGenerators() { return m_particleContactGenerators; }
	inline std::unordered_map<std::string, std::unique_ptr<IShapeGenerator>> & getShapeGenerators() { return m_shapeGenerators; }

    Particle* getParticle(std::string label)
    {
        return m_particles.at(label).get();
    }

    template<typename T = ParticleForceGenerator>
    T* getParticleForceGenerator(std::string label)
    {
        static_assert(std::is_base_of_v<ParticleForceGenerator, T>, "Must be an instanceof ParticleForceGenerator");
        return static_cast<T*>(m_particleForceGenerators.at(label).get());
    }

    template<typename T = ParticleContactGenerator>
    T* getParticleContactGenerator(std::string label)
    {
        static_assert(std::is_base_of_v<ParticleContactGenerator, T>, "Must be an instanceof ParticleContactGenerator");
        return static_cast<T*>(m_particleContactGenerators.at(label).get());
    }

    template<typename T = IShapeGenerator>
    T* getShapeGenerator(std::string label)
    {
        static_assert(std::is_base_of_v<IShapeGenerator, T>, "Must be an instanceof IShapeGenerator");
        return static_cast<T*>(m_shapeGenerators.at(label).get());
    }

private:
    std::unordered_map<std::string, std::unique_ptr<Particle>> m_particles;
    std::unordered_map<std::string, std::unique_ptr<ParticleForceGenerator>> m_particleForceGenerators;
    std::unordered_map<std::string, std::unique_ptr<ParticleContactGenerator>> m_particleContactGenerators;
    std::unordered_map<std::string, std::unique_ptr<IShapeGenerator>> m_shapeGenerators;
};

#endif // MPJVP_GAMESTATE
