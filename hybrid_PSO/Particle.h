#pragma once
class Swarm;
#include "Swarm.h"

class Particle
{
public:
	double* position;
	double* velocity;

	double* bestPosition;
	double bestFitness;
	double latestFitness;

	Particle();
	~Particle(void);
};

