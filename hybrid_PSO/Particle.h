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

	double _inertiaWeight;
	double _cognitiveWeight;
	double _socialWeight;
	double _mutationWeight;
};

