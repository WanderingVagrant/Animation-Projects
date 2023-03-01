#pragma once
#include "core.h"
#include "particle.h"
#include "PhysWorld.h"
class Triangle {
public:
	Particle& p1;
	Particle& p2;
	Particle& p3;
	glm::vec3 normal;
	Triangle(Particle& p1, Particle& p2, Particle& p3);
	void computeNorm();
	void computeForce() const;
};