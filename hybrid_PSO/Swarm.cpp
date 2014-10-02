#include "Swarm.h"
#include <limits>
#include "test_func.h"

void test_func(double*, double*, int, int,int);
double *OShift,*M,*y,*z,*x_bound;;
int ini_flag,n_flag,func_flag;

Swarm::Swarm(void)
	: random()
	, randomGenerator(std::mt19937(random()))
{
	//swarm settings
	size = 30;
	iterations = 10000;
	//solution range
	min_x = -100;
	max_x = 100;
	dimension = 10;
	max_velocity = 50;
	//PSO settings
	inertiaWeight = 0.7298;
	cognitiveWeight = 1.49618;
	socialWeight = 1.49618;
	//hybrid part settings
	mutationWeight = 0.005;
	crossoverRatio = 0.9;
	mutate = true;
	//benchmark function
	functionNumber = 10;
}

Swarm::~Swarm(void)
{
}

void Swarm::run()
{
	swarmBest = new double[dimension];
	particles = new Particle[size];
	fitness = new double;
	swarmBestFitness = std::numeric_limits<double>::max();
	currentIteration = -1;

	for(int i=0; i<size; i++)
	{
		particles[i].init(this, i);
	}
	for(currentIteration=0; currentIteration<iterations; currentIteration++)
	{
		for(int i=0; i<size; i++)
		{
			particles[i].update();
		}
	}
	std::cout << "Final GLOBAL best! " << swarmBestFitness << "\n";

	delete[] particles;
	delete[] fitness;
	delete[] swarmBest;
}

double Swarm::getFitness(Particle* p)
{
	test_func(p->position, fitness, dimension, 1, functionNumber); //l'ultimo int è il numero della funzione [1-28]

	return fitness[0];
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