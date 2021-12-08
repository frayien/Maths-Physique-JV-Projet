#ifndef MPJVP_GAMESTATE
#define MPJVP_GAMESTATE

#include <vector>
#include <memory>
#include <unordered_map>
#include "physics/Particle.hpp"
#include "physics/forces/RigidBody/RigidBodyForceGenerator.hpp"
#include "physics/forces/Particle/ParticleForceGenerator.hpp"
#include "physics/contacts/Particle/ParticleContactGenerator.hpp"
#include "render/shape/IShapeGenerator.hpp"
#include "physics/RigidBody.hpp"
#include "collision/BoundingVolumeSphere.hpp"
#include "physics/contacts/RigidBody/Primitive.hpp"

class GameState
{

public:
    GameState();
	virtual ~GameState();

    bool addParticle(const std::string & label, std::unique_ptr<Particle> && particle);
    bool addRigidBody(const std::string & label, std::unique_ptr<RigidBody> && rigidBody);
    bool addRigidBodyForceGenerator(const std::string& label, std::unique_ptr<RigidBodyForceGenerator>&& rigidBodyForceGenerator);
    bool addParticleForceGenerator(const std::string & label, std::unique_ptr<ParticleForceGenerator> && particleForceGenerator);
    bool addParticleContactGenerator(const std::string & label, std::unique_ptr<ParticleContactGenerator> && particleContactGenerator);
    bool addShapeGenerator(const std::string & label, std::unique_ptr<IShapeGenerator> && shapeGenerator);

    bool removeShapeGenerator(const std::string & label);

    inline std::unordered_map<std::string, std::unique_ptr<Particle>> & getParticles() { return m_particles; }
    inline std::unordered_map<std::string, std::unique_ptr<RigidBody>> & getRigidbodies() { return m_rigidbodies; }
	inline std::unordered_map<std::string, std::unique_ptr<ParticleForceGenerator>> & getParticleForceGenerators() { return m_particleForceGenerators; }
	inline std::unordered_map<std::string, std::unique_ptr<ParticleContactGenerator>> & getParticleContactGenerators() { return m_particleContactGenerators; }
	inline std::unordered_map<std::string, std::unique_ptr<IShapeGenerator>> & getShapeGenerators() { return m_shapeGenerators; }
    inline std::unordered_map<std::string, std::unique_ptr<RigidBodyForceGenerator>>& getRigidBodyForceGenerators() { return m_rigidBodyForceGenerators; }
    inline std::unordered_map<BoundingVolumeSphere*, std::vector<Primitive*>>& getLinksBetweenBoundingVolumesAndPrimitives() { return m_linksBetweenBoundingVolumesAndPrimitives; }
    inline std::vector<std::unique_ptr<BoundingVolumeSphere>>& getBoundingVolumeSphere() { return m_boundingVolumeSpheres; }
    inline std::vector<std::unique_ptr<Primitive>>& getPrimitives() { return m_primitives; }

    Particle* getParticle(std::string label)
    {
        return m_particles.at(label).get();
    }

    RigidBody* getRigidbody(std::string label)
    {
        return m_rigidbodies.at(label).get();
    }

    template<typename T = ParticleForceGenerator>
    T* getParticleForceGenerator(std::string label)
    {
        static_assert(std::is_base_of_v<ParticleForceGenerator, T>, "Must be an instanceof ParticleForceGenerator");
        return static_cast<T*>(m_particleForceGenerators.at(label).get());
    }

    template<typename T = RigidBodyForceGenerator>
    T* getRigidBodyForceGenerator(std::string label)
    {
        static_assert(std::is_base_of_v<RigidBodyForceGenerator, T>, "Must be an instanceof RigidBodyForceGenerator");
        return static_cast<T*>(m_rigidBodyForceGenerators.at(label).get());
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

    inline void clear()
    {
        m_particles.clear();
        m_rigidbodies.clear();
        m_particleForceGenerators.clear();
        m_rigidBodyForceGenerators.clear();
        m_particleContactGenerators.clear();
        m_shapeGenerators.clear();
        m_linksBetweenBoundingVolumesAndPrimitives.clear();
        m_boundingVolumeSpheres.clear();
        m_primitives.clear();
    }

private:
    std::unordered_map<std::string, std::unique_ptr<Particle>> m_particles;
    std::unordered_map<std::string, std::unique_ptr<RigidBody>> m_rigidbodies;
    std::unordered_map<std::string, std::unique_ptr<ParticleForceGenerator>> m_particleForceGenerators;
    std::unordered_map<std::string, std::unique_ptr<RigidBodyForceGenerator>> m_rigidBodyForceGenerators;
    std::unordered_map<std::string, std::unique_ptr<ParticleContactGenerator>> m_particleContactGenerators;
    std::unordered_map<std::string, std::unique_ptr<IShapeGenerator>> m_shapeGenerators;
    std::unordered_map<BoundingVolumeSphere*, std::vector<Primitive*>> m_linksBetweenBoundingVolumesAndPrimitives;
    std::vector<std::unique_ptr<BoundingVolumeSphere>> m_boundingVolumeSpheres;
    std::vector<std::unique_ptr<Primitive>> m_primitives;
};

#endif // MPJVP_GAMESTATE
