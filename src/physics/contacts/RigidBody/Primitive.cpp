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

Matrix33 Primitive::getRotation() const
{
	Matrix33 rotation
    {
        {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
        }
    };

	if (m_rigidBody != nullptr) rotation = m_rigidBody->getTranformMatrix().extractMatrix33();

	return m_offset.extractMatrix33() * rotation;
}