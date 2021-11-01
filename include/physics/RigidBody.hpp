#ifndef MPJVP_RIGIDBODY
#define MPJVP_RIGIDBODY

#include "physics/Vector3f.hpp"
#include "physics/Quaternion.hpp"
#include "physics/Matrix34.hpp"
#include <numeric>

class RigidBody {
public:
	RigidBody();
	RigidBody(Vector3f position, float mass, float damping);
	~RigidBody();

	// Modify position, orientation and velocities (linear and angular)
	void integrate(float duration);

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

	inline float getDamping() const { return m_damping; }
	inline void setDamping(float damping) { m_damping = damping; }

	inline Quaternion getQuaternion() const { return m_orientation; }
	inline void setQuaternion(Quaternion orientation) { m_orientation = orientation; }

	inline Vector3f getRotation() const { return m_rotation; }
	inline void setRotation(Vector3f rotation) { m_rotation = rotation; }
	inline void addRotation(Vector3f rotation) { m_rotation += rotation; }

	inline Matrix34 getTranformMatrix() const { return m_transformMatrix; }
	inline void setTransformMatrix(Matrix34 transformMatrix) { m_transformMatrix = transformMatrix; }

	friend std::ostream& operator<<(std::ostream& out, const RigidBody& a);


private:
	Vector3f m_position;
	Vector3f m_velocity;
	Vector3f m_totalForce;
	float m_inverseMass;
	float m_damping;

	// Orientation of the Rigid Body
	Quaternion m_orientation;

	// Angular velocity
	Vector3f m_rotation;

	// transform Matrix from the calcul between orientation and rotation
	Matrix34 m_transformMatrix;

	void calculateDerivedData();
};

#endif // MPJVP_RIGIDBODY