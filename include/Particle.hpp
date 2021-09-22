#ifndef MPJVP_PARTICLE
#define MPJVP_PARTICLE

#include <iostream>
#include <cmath>
#include <chrono>
#include <vector>

#include "Vector3f.hpp"

class Particle
{

public:
	Particle();
	Particle(Vector3f position, float mass, float damping);
	~Particle();

	void setMass(float mass);
	void integrate();
	void calculAcceleration(const std::vector<Vector3f>& forceList);

	inline Vector3f getPosition() const { return m_position; }
	inline Vector3f getVelocity() const { return m_velocity; }
	inline void setInitialVelocity(Vector3f initialVelocity) { m_velocity = initialVelocity; }
	inline Vector3f getAcceleration() const { return m_acceleration; }
	inline float getInverseMass() const { return m_inverseMass; }
	inline float getDamping() const { return m_damping; }

	friend std::ostream& operator<<(std::ostream& out, const Particle& a);

private :
	Vector3f m_position;
	Vector3f m_velocity;
	Vector3f m_acceleration;
	float m_inverseMass;
	float m_damping;
};

#endif // MPJVP_PARTICLE
