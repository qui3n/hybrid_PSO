#pragma once
class Swarm;
#include "Swarm.h"
#include <iostream>
#include <math.h>
#include <limits>

class Particle
{
public:
	double* position;
	double* velocity;

	double* bestPosition;
	double bestFitness;

	Particle();
	~Particle(void);
	void init(Swarm* swarm);
	void update();

private:
	void Particle::copyArray(double* src, double* dest);
	void Particle::checkBest();

	Swarm* motherSwarm;
};

