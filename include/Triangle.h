#pragma once
#include "core.h"
#include "particle.h"
#include "PhysWorld.h"
class Triangle {
public:
	Particle& p1;
	Particle& p2;
	Particle& p3;
	Triangle(Particle& p1, Particle& p2, Particle& p3);
};