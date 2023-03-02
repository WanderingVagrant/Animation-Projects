#include "..\include\Particle.h"

Particle::Particle(glm::vec3& position, glm::vec3& normal, float mass, bool top) : position(position), normal(normal), mass(mass)
{
	this->fixed = top;
	force = glm::vec3(0);
	velocity = glm::vec3(0);
}

void Particle::update(float t)
{
	if (fixed) {
		return;
	}

	//Add gravity and convert to acceleration
	glm::vec3 accel = force / mass;
	accel.y -= PhysWorld::gravity;
	
	//Semi-Implicit Euler
	velocity += accel * t;
	position += velocity * t;

	//Collisions
	if (position.y < 0) {
		position.y = 0;
	}

	//Zero out normal
	normal = glm::vec3(0);

	//Zero out the force
	force = glm::vec3(0);
}
