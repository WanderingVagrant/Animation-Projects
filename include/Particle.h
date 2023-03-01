#pragma once
#include "core.h"
#include "PhysWorld.h"

class Particle {
public:
	glm::vec3& position;
	glm::vec3& normal;
	glm::vec3 force;
	glm::vec3 velocity;
	float mass;
	bool fixed;
	Particle(glm::vec3 & position, glm::vec3 & normal, float mass, bool top = false);
	void update(float t);
};