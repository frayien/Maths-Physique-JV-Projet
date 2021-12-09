#ifndef MPJVP_PLANE
#define MPJVP_PLANE

#include "physics/contacts/RigidBody/Primitive.hpp"
#include "physics/Vector3f.hpp"

class Plane : public Primitive
{
public:
	Plane(RigidBody* rigidBody, Matrix34 offset, Vector3f normal, float thickness);
	~Plane() = default;

	inline Vector3f getNormal() const { return m_normal; }
	inline float getThickness() const { return m_thickness; }

private:
	Vector3f m_normal;
	float m_thickness; // probablement l'épaisseur du plan
};

#endif // MPJVP_PLANE