#ifndef MPJVP_PRIMITIVE
#define MPJVP_PRIMITIVE

#include "physics/Matrix34.hpp"
#include "physics/Vector3f.hpp"

class RigidBody;
class Primitive 
{
public:

	Primitive(RigidBody* rigidBody, Matrix34 offset);
	virtual ~Primitive() = default;

	Vector3f getPosition() const;
	Matrix33 getRotation() const;
	inline RigidBody* getRigidBody() const { return m_rigidBody; }
	inline Matrix34 getOffset() const { return m_offset; }

private:
	RigidBody* m_rigidBody;
	Matrix34 m_offset;
};

#endif // MPJVP_PRIMITIVE
