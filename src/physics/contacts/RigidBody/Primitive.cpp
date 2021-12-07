#include "physics/contacts/RigidBody/Primitive.hpp"

#include "physics/RigidBody.hpp"

Primitive::Primitive(RigidBody* rigidBody, Matrix34 offset) : 
	m_rigidBody{ rigidBody }, 
	m_offset{ offset }
{

}

Vector3f Primitive::getPosition() const
{
	Vector3f basePos = m_rigidBody != nullptr ? m_rigidBody->getPosition() : Vector3f{ 0.f, 0.f, 0.f };
	return m_offset * basePos;
}