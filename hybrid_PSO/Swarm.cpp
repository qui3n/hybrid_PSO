#include "Swarm.h"

Swarm::Swarm(void)
	: random()
	, randomGenerator(std::mt19937(random()))
{
	size = 100;
	iterations = 1000;

	min_x = 0;
	max_x = 100;
	dimension = 20;
	max_velocity = 50;

	inertiaWeight = 0.7298;
	cognitiveWeight = 1.49618;
	socialWeight = 1.49618;
	mutationWeight = 0;
	crossoverRatio = 1;

	mutate = true;

	swarmBest = new double[dimension];
	swarmBestFitness = std::numeric_limits<double>::max();
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

double Swarm::getRandomPosition()
{
	std::uniform_real_distribution<double> validPositionDistribution(min_x, max_x);
	return validPositionDistribution(randomGenerator);
}

double Swarm::getRandomVelocity()
{
	std::uniform_real_distribution<double> validVelocityDistribution(-max_velocity, max_velocity);
	return validVelocityDistribution(randomGenerator);
}

double Swarm::getRandomFactor()
{
	std::uniform_real_distribution<double> zeroToOne(0,1);
	return zeroToOne(randomGenerator);
}

int Swarm::getRandomIndex()
{
	std::uniform_int_distribution<int> validIndexDistribution(0, size-1);
	return validIndexDistribution(randomGenerator);
}