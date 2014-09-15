#include "Swarm.h"

Swarm::Swarm(void)
{
	size = 100;
	iterations = 600;

	min_x = 0;
	max_x = 100;
	dimension = 5;
	max_velocity = 50;

	inertiaWeight = 0.7298;
	cognitiveWeight = 1.49618;
	socialWeight = 1.49618;
	mutationWeight = 0.35;
	crossoverRatio = 0.9;

	mutate = false;

	swarmBest = new double[dimension];
	swarmBestFitness = std::numeric_limits<double>::max( );
}

Swarm::~Swarm(void)
{
}

void Swarm::run()
{
	particles = new Particle[size];

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