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
	//opzioni stormo
	size = 10;
	iterations = 10000;
	//opzioni campo delle soluzioni
	min_x = -100;
	max_x = 100;
	dimension = 10;
	max_velocity = 100;
	//opzioni PSO standard
	inertiaWeight = 0.7298;
	cognitiveWeight = 1.49618;
	socialWeight = 1.49618;
	//opzioni della parte ibrida
	mutationWeight = 0.5;
	crossoverRatio = 0.9;
	mutate = false;

	swarmBest = new double[dimension];
	swarmBestFitness = std::numeric_limits<double>::max();
}

Swarm::~Swarm(void)
{
}

void Swarm::run()
{
	particles = new Particle[size];
	fitness = new double;
	currentIteration = -1;

	for(int i=0; i<size; i++)
	{
		particles[i].init(this);
	}
	for(currentIteration=0; currentIteration<iterations; currentIteration++)
	{
		forcedMutantIndex = getRandomIndex();

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

/*
double Swarm::getFitness(Particle* p)
{
	double fitness = 0;
	for(int i=0; i<dimension; i++)
	{
		fitness += pow((p->position[i] - 20),2);
	}
	return sqrt(fitness);
}
*/

double Swarm::getFitness(Particle* p)
{
	test_func(p->position, fitness, dimension, 1, 7);

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