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
	double run();
	
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

	enum UpdateStrategy
	{
		PSO,
		HPSO,
		HPSOv2,
		HPSOv3,
		HPSORand
	};

private:
	void preRun();
	void postRun();

	double getFitness();
	double getFitness(Particle& p);
	double getFitness(double* position);
	double getRandomPosition();
	double getRandomVelocity();
	double getRandomFactor();
	int getRandomIndex();
	int getRandomDimensionIndex();

	void copyArray(double* src, double* dest);
	void initParticle(Particle& p);
	void updateBest(Particle& p);
	void (Swarm::*updateStrategy)();
	void setUpdateStrategy(UpdateStrategy ups);

	double* fitness;
	double* trialPSOParticlePosition;
	double* trialPSOParticleVelocity;
	double* trialDEParticle;
	double* trialPosition;
	double* trialVelocity;

	int currentParticleIndex;

	std::random_device random;
    std::mt19937 randomGenerator;

	//update strategies
	void PSOupdateParticle();
	void HPSOupdateParticle();
	void HPSOv2updateParticle();
	void HPSOv3updateParticle();
	void HPSORandupdateParticle();
	void reweight();
};