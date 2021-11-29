#include "physics/contacts/RigidBody/SphereAndSphereContact.hpp"

SphereAndSphereContact::SphereAndSphereContact(Sphere* sphereA, Sphere* sphereB) {
	m_spheres[0] = sphereA;
	m_spheres[1] = sphereB;
}

SphereAndSphereContact::~SphereAndSphereContact() {

}

float SphereAndSphereContact::getCurrentDistance() const {
	Vector3f particleDistance{ m_spheres[0]->getPosition() - m_spheres[1]->getPosition() };

	return particleDistance.norm();
}

void SphereAndSphereContact::addContact(std::vector<RigidBodyContact>& contacts) const {
	if (getCurrentDistance() > m_spheres[0]->getRadius() + m_spheres[1]->getRadius()) {
		Vector3f vectorToNormalize{ m_spheres[0]->getPosition() - m_spheres[1]->getPosition() };

		Vector3f extremitySphereA{ (m_spheres[1]->getPosition() - m_spheres[0]->getPosition()) - (-vectorToNormalize.normalize() * m_spheres[1]->getRadius()) };
		Vector3f extremitySphereB{ (m_spheres[0]->getPosition() - m_spheres[1]->getPosition()) - (vectorToNormalize.normalize() * m_spheres[0]->getRadius()) };

		contacts.emplace_back(m_spheres[0]->getRigidBody(), m_spheres[1]->getRigidBody(), std::abs(m_spheres[0]->getRadius() - (getCurrentDistance() - m_spheres[1]->getRadius())), vectorToNormalize.normalize(), (extremitySphereA + extremitySphereB) / 2.f, 0.95f, 0.95f);
	}
}