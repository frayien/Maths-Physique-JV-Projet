#ifndef FNES_PARTICLE
#define FNES_PARTICLE

#include <iostream>
#include <cmath>
#include "Vector3f.hpp"
#include <chrono>
#include <vector>

class Particle {

public:
	Particle();
	Particle(Vector3f position, float mass, float damping);
	~Particle();

	void setInverseMass(float mass);
	void integrate();
	void calculAcceleration(const std::vector<Vector3f>& forceList);

	inline Vector3f getPosition() const { return position; }
	inline Vector3f getVelocity() const { return velocity; }
	inline void setInitialVelocity(Vector3f initialVelocity) { velocity = initialVelocity; }
	inline Vector3f getAcceleration() const { return acceleration; }
	inline float getInverseMass() const { return inverseMass; }
	inline float getDamping() const { return damping; }

	friend std::ostream& operator<<(std::ostream& out, const Particle& a);

private :
	Vector3f position;
	Vector3f velocity;
	Vector3f acceleration;
	float inverseMass;
	float damping;
};

#endif // FNES_PARTICLE