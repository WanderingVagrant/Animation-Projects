#pragma once
#include "core.h"
#include "Particle.h"

class SpringDamper {
public:
	Particle& p1;
	Particle& p2;
	SpringDamper(Particle& p1, Particle& p2);
};