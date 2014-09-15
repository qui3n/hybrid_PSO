#include "Swarm.h"
#include <iostream>
#include <math.h>

Swarm::Swarm(void)
{
	size = 100;
	iterations = 2000;

	min_x = 0;
	max_x = 100;
	dimension = 30;
	max_velocity = 50;

	inertiaWeight = 0.5;
	cognitiveWeight = 1.49;
	socialWeight = 1.49;
	crossoverRatio = 1;

	swarmBest = new double[dimension];
	swarmBestFitness = 777777; // TODO: max_double
}

Swarm::~Swarm(void)
{
}

void Swarm::run()
{
	Particle particles[100]; // TODO: init w/ 'size'

	for(int i=0; i<size; i++)
	{
		particles[i].init(this);
	}
	for(currentIteration=0; currentIteration<iterations; currentIteration++)
	{
		for(int i=0; i<size; i++)
		{
			particles[i].update();
		}
	}
	std::cout << "Final GLOBAL best! " << swarmBestFitness << "\n";
}

double Swarm::getFitness(Particle* p)
{
	double fitness = 0;
	for(int i=0; i<dimension; i++)
	{
		fitness += pow((p->position[i] - 50),2);
	}
	return sqrt(fitness);
}