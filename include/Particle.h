#pragma once
#include "core.h"
#include "PhysWorld.h"

class Particle {
public:
	glm::vec3& position;
	glm::vec3& normal;
	float force;
	bool fixed;
	Particle(glm::vec3 & position, glm::vec3 & normal, bool top = false);
	void update();
};