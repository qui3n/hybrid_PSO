#pragma once
class Particle;
#include "Particle.h"

class Swarm
{
public:
	Swarm(void);
	~Swarm(void);
	void run();
	double getFitness(Particle* p);

	int dimension;
	int min_x, max_x, max_velocity;
	int iterations;
	int size;

	int currentIteration;

	double inertiaWeight;
	double cognitiveWeight;
	double socialWeight;

	double crossoverRatio;

	double* swarmBest;
	double swarmBestFitness;
};

