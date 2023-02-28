#include "..\include\Particle.h"

Particle::Particle(glm::vec3& position, glm::vec3& normal, bool top) : position(position), normal(normal)
{
	this->fixed = top;
}

void Particle::update()
{
}
