#pragma once
class Particle;
#include "Particle.h"
#include <random>
#include <iostream>
#include <math.h>
#include <limits>
#include <fstream>

class Swarm
{
public:
	Swarm(void);
	~Swarm(void);
	void run();
	double getFitness(Particle& p);
	double getRandomPosition();
	double getRandomVelocity();
	double getRandomFactor();
	int getRandomIndex();
	int getRandomDimensionIndex();

	int dimension;
	int min_x, max_x, max_velocity;
	int iterations;
	int size;

	int currentIteration;

	double inertiaWeight;
	double cognitiveWeight;
	double socialWeight;
	double mutationWeight;

	double crossoverRatio;

	double* swarmBest;
	double swarmBestFitness;

	Particle* particles;

	int functionNumber;

private:
	void updateParticle(Particle& p, int index);
	void copyArray(double* src, double* dest);
	void initParticle(Particle& p);
	void updateBest(Particle& p);

	double* fitness;
	double* trialPSOParticlePosition;
	double* trialPSOParticleVelocity;
	double* trialDEParticle;

	std::random_device random;
    std::mt19937 randomGenerator;
};