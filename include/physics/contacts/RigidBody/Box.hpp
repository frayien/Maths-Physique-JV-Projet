#ifndef MPJVP_BOX
#define MPJVP_BOX

#include "physics/contacts/RigidBody/Primitive.hpp"
#include "physics/Vector3f.hpp"

class Box : public Primitive
{
public:
	Box(RigidBody* rigidBody, Matrix34 offset, Vector3f halfSize);
	~Box() = default;

	inline Vector3f getHalfSize() const { return m_halfSize; }

private:
	Vector3f m_halfSize;
};

#endif // MPJVP_BOX
