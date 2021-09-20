#include "Particle.hpp"

Particle::Particle() {
	position = Vector3f::Vector3f();
	velocity = Vector3f::Vector3f();
	acceleration = Vector3f::Vector3f();
	inverseMass = 0.001;
	damping = 0;
}

Particle::Particle(Vector3f position, float mass, float damping) : damping(damping), position(position){
	inverseMass = 1 / mass;
}

Particle::~Particle() {
}

void Particle::setInverseMass(float mass) {
	inverseMass = 1 / mass;
}

void Particle::integrate() {

	using namespace std::chrono_literals;

	const std::chrono::nanoseconds TIMESTEP(1s);
	const float DELTA_TIME = TIMESTEP.count() / 1'000'000'000;

	using clock = std::chrono::high_resolution_clock;

	std::chrono::nanoseconds lag(0ns);
	auto base_time = clock::now();

	std::vector<Vector3f> forceList({ Vector3f(0, -9.81, 0) });

	while (true) {
		auto current_time = clock::now();
		auto frame_time = current_time - base_time;
		base_time = current_time;
		lag += std::chrono::duration_cast<std::chrono::nanoseconds>(frame_time);

		while (lag >= TIMESTEP) {
			lag -= TIMESTEP;

			calculAcceleration(forceList);
			position += velocity * DELTA_TIME + (DELTA_TIME * DELTA_TIME * acceleration / 2);
			velocity = velocity * damping + acceleration * DELTA_TIME;
			
			std::cout << "================" << std::endl;
			std::cout << "acceleration = " << acceleration << std::endl;
			std::cout << "velocity = " << velocity << std::endl;
			std::cout << "position = " << position << std::endl;
			std::cout << "================" << std::endl << std::endl;
		}
	}
}

void Particle::calculAcceleration(const std::vector<Vector3f> & forceList) {
	Vector3f totalForce{ 0, 0, 0 };
	for (int i = 0; i < forceList.size(); i++) {
		totalForce += forceList[i];
	}
	acceleration = inverseMass * totalForce;
}

std::ostream& operator<<(std::ostream& out, const Particle& a)
{
	return out << "position : " << a.position << std::endl << "velocity : " << a.velocity << std::endl << "acceleration : " << a.acceleration << std::endl << "inverseMass : " << a.inverseMass << std::endl << "damping : " << a.damping << std::endl << std::endl;
}