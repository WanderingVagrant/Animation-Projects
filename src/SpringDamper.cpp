#include "..\include\SpringDamper.h"

SpringDamper::SpringDamper(Particle& p1, Particle& p2):p1(p1), p2(p2)
{
	rest = glm::distance(p1.position, p2.position);
	k = PhysWorld::sconstant;
	kd = PhysWorld::dconstant;
}

void SpringDamper::ComputeForce() const
{
	//SpringDamp
	float dist = glm::distance(p1.position, p2.position);
	glm::vec3 dir = glm::normalize(p2.position - p1.position);
	float vclose = glm::dot(p1.velocity - p2.velocity, dir);

	glm::vec3 force = (k * (dist - rest) - kd*vclose) * dir ;

	p1.force += force;
	p2.force -= force;
}
