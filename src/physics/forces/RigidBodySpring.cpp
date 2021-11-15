#include "physics/forces/RigidBodySpring.hpp"

RigidBodySpring::RigidBodySpring(Vector3f bodyAnchor, RigidBody * otherRigidBody, Vector3f otherBodyAnchor, float k, float restLength) :
    m_bodyAnchor{bodyAnchor},
    m_otherRigidBody{otherRigidBody},
    m_otherBodyAnchor{otherBodyAnchor},
    m_k{k},
    m_restLength{restLength}
{

}

RigidBodySpring::~RigidBodySpring()
{

}

void RigidBodySpring::updateForce(RigidBody* rigidBody, float duration)
{
    rigidBody->setIsResting(false);

    Vector3f worldBodyAnchor = rigidBody->getTranformMatrix() * m_bodyAnchor;
    Vector3f worldOtherBodyAnchor = m_otherRigidBody->getTranformMatrix() * m_otherBodyAnchor;

    Vector3f distance = worldBodyAnchor - worldOtherBodyAnchor;
    Vector3f force = - m_k * (distance.norm() - m_restLength) * distance.normalize();

    rigidBody->addForceAtBodyPoint(force, m_bodyAnchor);
}