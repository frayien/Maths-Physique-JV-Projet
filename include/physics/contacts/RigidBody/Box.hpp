#ifndef MPJVP_BOX
#define MPJVP_BOX

#include "physics/contacts/RigidBody/Primitive.hpp"
#include "physics/Vector3f.hpp"

class Box : public Primitive
{
	inline Vector3f getNormalX() const { return m_normalX; }
	inline Vector3f getNormalY() const { return m_normalY; }
	inline Vector3f getSize() const { return m_size; }

private:
	Vector3f m_normalX;
	Vector3f m_normalY;
	Vector3f m_size;
};

#endif // MPJVP_BOX
