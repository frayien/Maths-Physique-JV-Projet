#include "physics/contacts/RigidBody/Primitive.hpp"

Primitive::Primitive(RigidBody* rigidBody, Matrix34 offset) : 
	m_rigidBody{ rigidBody }, 
	m_offset{ offset }
{

}

Primitive::~Primitive() {

}