#include "Particle.hpp"

Particle::Particle()
{
	m_inverseMass = 1.0f;
	m_damping = 0.001f;
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

void Particle::integrate()
{
	using namespace std::chrono_literals;

	const std::chrono::nanoseconds TIMESTEP(1s);
	const float DELTA_TIME = TIMESTEP.count() / 1'000'000'000;

	using clock = std::chrono::high_resolution_clock;

	std::chrono::nanoseconds lag(0ns);
	auto base_time = clock::now();

	std::vector<Vector3f> forceList = { Vector3f(0, -9.81, 0) };

	while (true)
	{
		auto current_time = clock::now();
		auto frame_time = current_time - base_time;
		base_time = current_time;
		lag += std::chrono::duration_cast<std::chrono::nanoseconds>(frame_time);

		while (lag >= TIMESTEP)
		{
			lag -= TIMESTEP;

			//Code pour print les coordonnées Y et X d'une particule

			/*std::string coordYString = std::to_string(m_position.getY());
			std::string coordXString = std::to_string(m_position.getX());
			std::replace(coordYString.begin(), coordYString.end(), '.', ',');
			std::replace(coordXString.begin(), coordXString.end(), '.', ',');

			myFile << coordYString << ";" << coordXString << "\n";*/

			calculAcceleration(forceList);
			m_position += m_velocity * DELTA_TIME + (DELTA_TIME * DELTA_TIME * m_acceleration / 2);
			m_velocity = m_velocity * m_damping + m_acceleration * DELTA_TIME;

			
			
			std::cout << "================" << std::endl;
			std::cout << "acceleration = " << m_acceleration << std::endl;
			std::cout << "velocity = " << m_velocity << std::endl;
			std::cout << "position = " << m_position << std::endl;
			std::cout << "================" << std::endl << std::endl;
		}
	}
}

void Particle::calculAcceleration(const std::vector<Vector3f> & forceList)
{
	Vector3f totalForce{ 0, 0, 0 };
	for (int i = 0; i < forceList.size(); i++)
	{
		totalForce += forceList[i];
	}
	m_acceleration = m_inverseMass * totalForce;
}

std::ostream& operator<<(std::ostream& out, const Particle& a)
{
	return out << "position : "     << a.m_position     << std::endl 
	           << "velocity : "     << a.m_velocity     << std::endl 
			   << "acceleration : " << a.m_acceleration << std::endl 
			   << "inverseMass : "  << a.m_inverseMass  << std::endl 
			   << "damping : "      << a.m_damping      << std::endl << std::endl;
}