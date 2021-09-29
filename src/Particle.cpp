#include "Particle.hpp"

Particle::Particle()
{
	m_inverseMass = 1.0f;
	m_damping = 0.999f;
}

Particle::Particle(Vector3f position, float mass, float damping) : 
	m_damping(damping), 
	m_position(position)
{
	m_inverseMass = 1.0f / mass;
}

Particle::~Particle()
{
}

void Particle::setMass(float mass)
{
	m_inverseMass = 1.0f / mass;
}

void Particle::integrate(float DELTA_TIME)
{
			// Code for debugging (we print the coordinates of the particle)
			/*std::string coordYString = std::to_string(m_position.getY());
			std::string coordXString = std::to_string(m_position.getX());
			std::replace(coordYString.begin(), coordYString.end(), '.', ',');
			std::replace(coordXString.begin(), coordXString.end(), '.', ',');

			myFile << coordYString << ";" << coordXString << "\n";*/


			// We update the acceleration according to the forces' list, then we update the position and the velocity
			calculateAcceleration();
			m_position += m_velocity * DELTA_TIME + (DELTA_TIME * DELTA_TIME * m_acceleration / 2.0f);
			m_velocity = m_velocity * m_damping + m_acceleration * DELTA_TIME;
			/*
			std::cout << "================" << std::endl;
			std::cout << "acceleration = " << m_acceleration << std::endl;
			std::cout << "velocity = " << m_velocity << std::endl;
			std::cout << "position = " << m_position << std::endl;
			std::cout << "================" << std::endl << std::endl;
			*/
}

void Particle::calculateAcceleration()
{
	m_acceleration = m_inverseMass * m_totalForce;
}

std::ostream& operator<<(std::ostream& out, const Particle& a)
{
	return out << "position : "     << a.m_position     << std::endl 
	           << "velocity : "     << a.m_velocity     << std::endl 
			   << "acceleration : " << a.m_acceleration << std::endl 
			   << "inverseMass : "  << a.m_inverseMass  << std::endl 
			   << "damping : "      << a.m_damping      << std::endl << std::endl;
}