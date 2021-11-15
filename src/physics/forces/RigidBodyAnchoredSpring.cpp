#include "physics/forces/RigidBodyAnchoredSpring.hpp"

RigidBodyAnchoredSpring::RigidBodyAnchoredSpring(Vector3f bodyAnchor, Vector3f anchor, float k, float restLength) :
    m_bodyAnchor{bodyAnchor},
    m_anchor{anchor},
    m_k{k},
    m_restLength{restLength}
{

}

RigidBodyAnchoredSpring::~RigidBodyAnchoredSpring()
{

}

void RigidBodyAnchoredSpring::updateForce(RigidBody* rigidBody, float duration)
{
    rigidBody->setIsResting(false);

    Vector3f worldBodyAnchor = rigidBody->getTranformMatrix() * m_bodyAnchor;

    Vector3f distance = worldBodyAnchor - m_anchor;
    Vector3f force = - m_k * (distance.norm() - m_restLength) * distance.normalize();

    rigidBody->addForceAtBodyPoint(force, m_bodyAnchor);
}