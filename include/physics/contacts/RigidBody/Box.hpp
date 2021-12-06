#ifndef MPJVP_BOX
#define MPJVP_BOX

#include "physics/contacts/RigidBody/Primitive.hpp"
#include "physics/Vector3f.hpp"

class Box : public Primitive
{
	inline Vector3f getHalfSizeX() const { return m_halfSizeX; }
	inline Vector3f getHalfSizeY() const { return m_halfSizeY; }

private:
	Vector3f m_halfSizeX;
	Vector3f m_halfSizeY;
};

#endif // MPJVP_BOX
