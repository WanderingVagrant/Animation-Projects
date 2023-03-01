#pragma once
#include "core.h"
#include "Particle.h"

class SpringDamper {
public:
	Particle& p1;
	Particle& p2;
	float rest;
	float k;
	float kd;
	SpringDamper(Particle& p1, Particle& p2);
	void ComputeForce() const;
};