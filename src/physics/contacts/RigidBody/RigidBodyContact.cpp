#include "physics/contacts/RigidBody/RigidBodyContact.hpp"

RigidBodyContact::RigidBodyContact(RigidBody* rigidBodyA, RigidBody* rigidBodyB, float penetration, Vector3f normal, Vector3f position, float friction, float restitution) :
    m_rigidbodies{rigidBodyA, rigidBodyB},
    m_penetration(penetration),
    m_normal(normal),
    m_position(position),
    m_friction(friction),
    m_restitution(restitution)
{
}

RigidBodyContact::~RigidBodyContact()
{
}

void RigidBodyContact::calculateInternals()
{
    // Check if the first rigidbody is null and swap if it is
    if (!m_rigidbodies[0]) swapRigidbodies();

    // To be sure, check if the first rigidbody is not null after swapping
    if (!m_rigidbodies[0]) return;

    // Contact basis
    calculateContactBasis();

    m_relativeContactPositions[0] = m_position - m_rigidbodies[0]->getPosition();
    if (m_rigidbodies[1])
    {
        m_relativeContactPositions[1] = m_position - m_rigidbodies[1]->getPosition();
    }

    // Find the relative velocity of the rigidbodies at the contact point
    m_contactVelocity = calculateLocalVelocity(0);
    if (m_rigidbodies[1])
    {
        m_contactVelocity -= calculateLocalVelocity(1);
    }

    // Calculate the desired change in velocity for resolution
    calculateDesiredDeltaVelocity();
}

void RigidBodyContact::applyPositionChange()
{
    float totalInertia = 0.0f;

    for (int i = 0; i < 2; ++i)
    {
        if (!m_rigidbodies[i]) continue;

        Matrix33 inverseInertiaTensor = m_rigidbodies[i]->getInverseInertiaTensor();

        // Angular inertia
        Vector3f angularInertiaWorld = m_relativeContactPositions[i].crossProduct(m_normal);
        angularInertiaWorld = inverseInertiaTensor * angularInertiaWorld;
        angularInertiaWorld = angularInertiaWorld.crossProduct(m_relativeContactPositions[i]);
        m_angularInertia[i] = angularInertiaWorld.dotProduct(m_normal);

        // Linear inertia
        m_lineraInertia[i] = m_rigidbodies[i]->getInverseMass();

        // Total inertia
        totalInertia += m_lineraInertia[i] + m_angularInertia[i];
    }

    float inverseInertia = 1.0f / totalInertia;

    m_linearMove[0] =  m_penetration * m_lineraInertia[0] * inverseInertia;
    m_linearMove[1] = -m_penetration * m_lineraInertia[1] * inverseInertia;
    m_angularMove[0] =  m_penetration * m_angularInertia[0] * inverseInertia;
    m_angularMove[1] = -m_penetration * m_angularInertia[1] * inverseInertia;

    for (int i = 0; i < 2; ++i)
    {
        if (!m_rigidbodies[i]) continue;

        // Check angular move limit
        float angularLimitConstant = 0.2f;
        float angularLimit = angularLimitConstant * m_relativeContactPositions[i].norm();
        if (glm::abs(m_angularMove[i]) > angularLimit)
        {
            float totalMove = m_linearMove[i] + m_angularMove[i];

            // Set the angular move with the same sign as before
            if (m_angularMove[i] >= 0)
            {
                m_angularMove[i] = angularLimit;
            }
            else
            {
                m_angularMove[i] = -angularLimit;
            }

            // Make the linear move take the extra move amount
            m_linearMove[i] = totalMove - m_angularMove[i];
        }

        // Applying linear movement
        m_rigidbodies[i]->setPosition(m_rigidbodies[i]->getPosition() + m_normal * m_linearMove[i]);

        // Applying angular movement
        Matrix33 inverseInertiaTensor = m_rigidbodies[i]->getInverseInertiaTensor();

        Vector3f impulsiveTorque = m_relativeContactPositions[i].crossProduct(m_normal);
        Vector3f impulsePerMove = inverseInertiaTensor * impulsiveTorque;

        Vector3f rotationPerMove;
        if (m_angularInertia[i] == 0.0f)
        {
            rotationPerMove = {0.0f, 0.0f, 0.0f};
        }
        else
        {
            rotationPerMove = impulsePerMove / m_angularInertia[i];
        }

        m_rotationAmount[i] = rotationPerMove * m_angularMove[i];

        Quaternion rigidbodyQuaternion = m_rigidbodies[i]->getQuaternion();
        Quaternion rotationQuaternion{ 0.0f, m_rotationAmount[i].getX(), m_rotationAmount[i].getY(), m_rotationAmount[i].getZ() };
        rigidbodyQuaternion = rigidbodyQuaternion + (1.0f / 2.0f) * rotationQuaternion * rigidbodyQuaternion;
        m_rigidbodies[i]->setQuaternion(rigidbodyQuaternion);
    }
}

void RigidBodyContact::applyVelocityChange()
{
    Vector3f deltaVelWorld = m_relativeContactPositions[0].crossProduct(m_normal);
    deltaVelWorld = m_rigidbodies[0]->getInverseInertiaTensor() * deltaVelWorld;
    deltaVelWorld = deltaVelWorld.crossProduct(m_relativeContactPositions[0]);

    float deltaVelocity = deltaVelWorld.dotProduct(m_normal);
    deltaVelocity += m_rigidbodies[0]->getInverseMass();

    if (m_rigidbodies[1])
    {
        deltaVelWorld = m_relativeContactPositions[1].crossProduct(m_normal);
        deltaVelWorld = m_rigidbodies[1]->getInverseInertiaTensor() * deltaVelWorld;
        deltaVelWorld = deltaVelWorld.crossProduct(m_relativeContactPositions[1]);

        deltaVelocity += deltaVelWorld.dotProduct(m_normal);

        deltaVelocity += m_rigidbodies[1]->getInverseMass();
    }


    Vector3f impulseContact{
        m_desiredDeltaVelocity / deltaVelocity,
        0.0f,
        0.0f
    };

    Vector3f impulse = m_contactToWorld * impulseContact;

    // First rigidbody
    Vector3f velocityChange = impulse * m_rigidbodies[0]->getInverseMass();
    Vector3f impulsiveTorque = impulse.crossProduct(m_relativeContactPositions[0]);
    Vector3f rotationChange = m_rigidbodies[0]->getInverseInertiaTensor() * impulsiveTorque;

    // Apply
    m_rigidbodies[0]->setVelocity(m_rigidbodies[0]->getVelocity() + velocityChange);
    m_rigidbodies[0]->setAngularVelocity(m_rigidbodies[0]->getAngularVelocity() + rotationChange);

    // Second rigidbody
    if (!m_rigidbodies[1]) return;

    impulse *= -1.0f;

    velocityChange = impulse * m_rigidbodies[1]->getInverseMass();
    impulsiveTorque = impulse.crossProduct(m_relativeContactPositions[1]);
    rotationChange = m_rigidbodies[1]->getInverseInertiaTensor() * impulsiveTorque;

    // Apply
    m_rigidbodies[1]->setVelocity(m_rigidbodies[1]->getVelocity() + velocityChange);
    m_rigidbodies[1]->setAngularVelocity(m_rigidbodies[1]->getAngularVelocity() + rotationChange);
}

// //////////// friends //////////// //

std::ostream& operator<<(std::ostream & out, const RigidBodyContact & c)
{
    out << "[\n" << "\t - Point d'impact : " << c.m_position << std::endl;
    out << "\t - Normale : " << c.m_normal << std::endl;
    out << "\t - Interpenetration : " << c.m_penetration << std::endl;
    out << "]";

    return out;
}

void RigidBodyContact::calculateContactBasis()
{
    Vector3f contactTangent[2];

    // Check whether the Z axis is nearer to the X or Y axis
    if (glm::abs(m_normal.getX()) > glm::abs(m_normal.getY()))
    {
        // Scaling factor to ensure the results are normalized
        const float s = 1.0f / glm::sqrt(m_normal.getZ() * m_normal.getZ() + m_normal.getX() * m_normal.getX());

        // The new X axis is at right angle to the world Y axis
        contactTangent[0].setX(m_normal.getZ() * s);
        contactTangent[0].setY(0.0f);
        contactTangent[0].setZ(-m_normal.getX() * s);

        // The new Y axis is at right angle to the new X and Z axes
        contactTangent[1].setX(m_normal.getY() * contactTangent[0].getX());
        contactTangent[1].setY(m_normal.getZ() * contactTangent[0].getX() - m_normal.getX() * contactTangent[0].getZ());
        contactTangent[1].setZ(-m_normal.getY() * contactTangent[0].getX());
    }
    else
    {
        // Scaling factor to ensure the results are normalized
        const float s = 1.0f / glm::sqrt(m_normal.getZ() * m_normal.getZ() + m_normal.getY() * m_normal.getY());

        // The new X axis is at right angle to the world X axis
        contactTangent[0].setX(0.0f);
        contactTangent[0].setY(-m_normal.getZ() * s);
        contactTangent[0].setZ(m_normal.getY() * s);

        // The new Y axis is at right angle to the new X and Z axes
        contactTangent[1].setX(m_normal.getY() * contactTangent[0].getZ() - m_normal.getZ() * contactTangent[0].getY());
        contactTangent[1].setY(-m_normal.getX() * contactTangent[0].getZ());
        contactTangent[1].setZ(m_normal.getX() * contactTangent[0].getY());
    }

    // Make the Matrix33 from the vectors
    m_contactToWorld = Matrix33
    {
        {
            m_normal.getX(), contactTangent[0].getX(), contactTangent[1].getX(),
            m_normal.getY(), contactTangent[0].getY(), contactTangent[1].getY(),
            m_normal.getZ(), contactTangent[0].getZ(), contactTangent[1].getZ()
        }
    };
}

void RigidBodyContact::swapRigidbodies()
{
    m_normal *= -1.0f;
    RigidBody* tmp = m_rigidbodies[0];
    m_rigidbodies[0] = m_rigidbodies[1];
    m_rigidbodies[1] = tmp;
}

Vector3f RigidBodyContact::calculateLocalVelocity(int rigidbodyIndex)
{
    Vector3f velocity = m_rigidbodies[rigidbodyIndex]->getVelocity().crossProduct(m_relativeContactPositions[rigidbodyIndex]);
    velocity += m_rigidbodies[rigidbodyIndex]->getVelocity();

    // Turn velocity into contact coordinates
    Vector3f contactVelocity = m_contactToWorld.inverse() * velocity;

    return contactVelocity;
}

void RigidBodyContact::calculateDesiredDeltaVelocity()
{
    // TODO : Calculate velocity from acceleration (need to store acceleration in rigidbody)
    float velocityFromAcceleration = 0.0f; // TODO

    if (m_rigidbodies[1])
    {
        velocityFromAcceleration -= 0.0f; // TODO
    }

    // If the velocity is very slow, limit the restitution
    float restitution = m_restitution;
    float velocityLimit = 0.2f;
    if (glm::abs(m_contactVelocity.getX()) < velocityLimit) restitution = 0.0f;

    m_desiredDeltaVelocity = -m_contactVelocity.getX() - restitution * (m_contactVelocity.getX() - velocityFromAcceleration);
}