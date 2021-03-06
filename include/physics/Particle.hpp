#ifndef MPJVP_PARTICLE
#define MPJVP_PARTICLE

#include <iostream>
#include <cmath>
#include <vector>
#include <numeric>
#include "physics/Vector3f.hpp"

class Particle
{

public:
	Particle();
	Particle(Vector3f position, float mass, float damping, bool isResting = false, float radius = 0.2f);
	~Particle();

	void integrate(float deltaTime);

	inline Vector3f getPosition() const { return m_position; }
	inline void setPosition(Vector3f pos) { m_position = pos; }
	inline void translate(Vector3f trans) { m_position += trans; }

	inline Vector3f getVelocity() const { return m_velocity; }
	inline void setVelocity(Vector3f initialVelocity) { m_velocity = initialVelocity; }
	inline void addVelocity(Vector3f vel) { m_velocity += vel; }

	inline Vector3f getTotalForce() const { return m_totalForce; }
	inline void setTotalForce(Vector3f totalForce) { m_totalForce = totalForce; }
	inline void addForce(Vector3f force) { m_totalForce += force; }

	inline float getInverseMass() const { return m_inverseMass; }
	inline float getMass() const { return m_inverseMass == 0.0f ? std::numeric_limits<float>().max() : (1.0f / m_inverseMass); }
	inline void setMass(float mass) { m_inverseMass = 1.0f / mass; }
	inline void setInverseMass(float inverseMass) { m_inverseMass = inverseMass; }

	inline float getRadius() const { return m_radius; }
	inline void setRadius(float radius) { m_radius = radius; }

	inline float getDamping() const { return m_damping; }
	inline void setDamping(float damping) { m_damping = damping; }

	inline bool getIsResting() const { return m_isResting; }
	inline void setIsResting(bool isResting) { m_isResting = isResting; }

	friend std::ostream& operator<<(std::ostream& out, const Particle& a);

private :
	Vector3f m_position;
	Vector3f m_velocity;
	Vector3f m_totalForce;
	float m_inverseMass;
	float m_damping;
	float m_radius;
	bool m_isResting;
};

#endif // MPJVP_PARTICLE
