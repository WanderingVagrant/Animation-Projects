#include "..\include\Triangle.h"

Triangle::Triangle(Particle& p1, Particle& p2, Particle& p3): p1(p1), p2(p2), p3(p3)
{
	normal = glm::vec3(0);
	computeNorm();
}

void Triangle::computeNorm()
{
	normal = glm::normalize(glm::cross(p2.position - p1.position, p3.position - p1.position));
}

void Triangle::computeForce()
{
	computeNorm();
	glm::vec3 vel = ((p1.velocity + p2.velocity + p3.velocity) * (1.0f / 3.0f))- PhysWorld::windmag * glm::normalize(PhysWorld::winddir);
	if (glm::length(vel) == 0) {
		return;
	}

	float area = (1.0f / 2.0f) * glm::dot(glm::normalize(vel), normal)*glm::length(glm::cross(p2.position - p1.position, p3.position - p1.position));
	glm::vec3 partforce = (1.0f/3.0f)*(-1.0f / 2.0f) * PhysWorld::density * glm::length(vel) * glm::length(vel) * PhysWorld::drag * area*normal;
	p1.force += partforce;
	p2.force += partforce;
	p3.force += partforce;
}
