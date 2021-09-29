#ifndef MPJVP_PARTICLE
#define MPJVP_PARTICLE

#include <iostream>
#include <cmath>
#include <vector>
#include "Vector3f.hpp"

class Particle
{

public:
	Particle();
	Particle(Vector3f position, float mass, float damping);
	~Particle();

	void setMass(float mass);
	void integrate(float DELTA_TIME);
	void calculateAcceleration();

	inline Vector3f getPosition() const { return m_position; }
	inline Vector3f getVelocity() const { return m_velocity; }
	inline void setVelocity(Vector3f initialVelocity) { m_velocity = initialVelocity; }
	inline void setPosition(Vector3f pos) { m_position = pos; }
	inline void setDamping(float damping) { m_damping = damping; }
	inline void setTotalForce(Vector3f totalForce) { m_totalForce = totalForce; }
	inline Vector3f getAcceleration() const { return m_acceleration; }
	inline float getInverseMass() const { return m_inverseMass; }
	inline float getDamping() const { return m_damping; }
	inline Vector3f getTotalForce() const { return m_totalForce; }

	friend std::ostream& operator<<(std::ostream& out, const Particle& a);

private :
	Vector3f m_position;
	Vector3f m_velocity;
	Vector3f m_acceleration;
	Vector3f m_totalForce;
	float m_inverseMass;
	float m_damping;
};

#endif // MPJVP_PARTICLE
