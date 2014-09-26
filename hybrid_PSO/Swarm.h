#pragma once
class Particle;
#include "Particle.h"
#include <random>

class Swarm
{
public:
	Swarm(void);
	~Swarm(void);
	void run();
	double getFitness(Particle* p);
	double getRandomPosition();
	double getRandomVelocity();
	double getRandomFactor();
	int getRandomIndex();

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

	bool mutate;
	int forcedMutantIndex;

private:
	double* fitness;

	//random stuff
	std::random_device random;
    std::mt19937 randomGenerator;

	int functionNumber;
};