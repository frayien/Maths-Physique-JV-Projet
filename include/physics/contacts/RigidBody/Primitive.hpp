#ifndef MPJVP_PRIMITIVE
#define MPJVP_PRIMITIVE

#include "physics/RigidBody.hpp"
#include "physics/Matrix34.hpp"
#include "physics/Vector3f.hpp"

class Primitive 
{
public:

	Primitive(RigidBody* rigidBody, Matrix34 offset);
	~Primitive();

	inline Vector3f getPosition(){ return m_offset * m_rigidBody->getPosition(); }
	inline RigidBody* getRigidBody() { return m_rigidBody; }
	inline Matrix34 getOffset() { return m_offset; }

private:
	RigidBody* m_rigidBody;
	Matrix34 m_offset;
};

#endif // MPJVP_PRIMITIVE