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
	max_velocity = 40;
	//PSO settings
	inertiaWeight = 0.7298;
	cognitiveWeight = 1.49618;
	socialWeight = 1.49618;
	//DE settings
	mutationWeight = 0.005;
	crossoverRatio = 0.9;
	//benchmark function
	functionNumber = 10;
}

Swarm::~Swarm(void)
{
}

void Swarm::run()
{
	trialPSOParticlePosition = new double[dimension];
	trialPSOParticleVelocity = new double[dimension];
	trialDEParticle = new double[dimension];
	trialPosition = new double[dimension];
	trialVelocity = new double[dimension];
	particles = new Particle[size];
	fitness = new double;

	swarmBest = new double[dimension];
	swarmBestFitness = std::numeric_limits<double>::max();
	currentIteration = -1;

	for(int i=0; i<size; i++)
	{
		initParticle(particles[i]);
	}
	for(currentIteration=0; currentIteration<iterations; currentIteration++)
	{
		for(int i=0; i<size; i++)
		{
			updateParticle(particles[i], i);
		}
	}
	//std::cout << "Final GLOBAL best! " << swarmBestFitness << "\n";

	delete[] particles;
	delete[] fitness;
	delete[] swarmBest;
	delete[] trialPSOParticlePosition;
	delete[] trialPSOParticleVelocity;
	delete[] trialDEParticle;
}

double Swarm::getFitness(Particle& p)
{
	test_func(p.position, fitness, dimension, 1, functionNumber);

	return fitness[0];
}

double Swarm::getFitness(double* position)
{
	test_func(position, fitness, dimension, 1, functionNumber);

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

int Swarm::getRandomDimensionIndex()
{
	std::uniform_int_distribution<int> validIndexDistribution(0, dimension-1);
	return validIndexDistribution(randomGenerator);
}

void Swarm::copyArray(double* src, double* dest)
{
	for(int i = 0; i < this->dimension; i++)
	{
		dest[i] = src[i];
	}
}

void Swarm::updateParticle(Particle& p, int index)
{
	copyArray(p.position, trialPSOParticlePosition);
	copyArray(p.velocity, trialPSOParticleVelocity);
	copyArray(p.position, trialDEParticle);

	// PSO
	for(int i=0; i<dimension; i++)
	{
		trialPSOParticleVelocity[i] *= inertiaWeight;
		trialPSOParticleVelocity[i] += cognitiveWeight * getRandomFactor() * ( p.bestPosition[i] - p.position[i] );
		trialPSOParticleVelocity[i] += socialWeight * getRandomFactor() * ( swarmBest[i] - p.position[i] );
	}
	for(int i=0; i<dimension; i++)
	{
		trialPSOParticlePosition[i] += trialPSOParticleVelocity[i];
		if(trialPSOParticlePosition[i] < min_x)
		{
			trialPSOParticlePosition[i] = min_x;
		}
		if(trialPSOParticlePosition[i] > max_x)
		{
			trialPSOParticlePosition[i] = max_x;
		}
	}

	// DE
	int randomParticle1, randomParticle2;
	do
	{
		randomParticle1 = getRandomIndex();
	}while(randomParticle1 == index);

	do
	{
		randomParticle2 = getRandomIndex();
	}while(randomParticle2 == index || randomParticle2 == randomParticle1);
	for(int i=0; i<dimension; i++)
	{
		if(getRandomFactor() < crossoverRatio)
		{
			trialDEParticle[i] += getRandomFactor() * mutationWeight * ( particles[randomParticle1].position[i] - particles[randomParticle2].position[i] );
		}
	}

	// Cross-Over
	for(int i=0; i<dimension; i++)
	{
		double r = getRandomFactor();
		double oldPosition = p.position[i];

		if(r < 0.9)
		{
			trialPosition[i] = trialPSOParticlePosition[i];
		}else 
		{
			trialPosition[i] = trialDEParticle[i];
		}

		trialVelocity[i] = p.position[i] - oldPosition;
	}
	
	// Selection
	double newFitness = getFitness(trialPosition);
	if(newFitness < p.bestFitness)
	{
		p.bestFitness = newFitness;
		copyArray(p.position, p.bestPosition);
		//std::cout << "New LOCAL best! " << p->bestFitness << "\n";
		if(p.bestFitness < swarmBestFitness)
		{
			swarmBestFitness = p.bestFitness;
			copyArray(p.bestPosition, swarmBest);
			//std::cout << "New GLOBAL best! " << p.bestFitness << " (" << currentIteration << ") \n";
		}
		copyArray(trialPosition, p.position);
		copyArray(trialVelocity, p.velocity);
		p.latestFitness = newFitness;
	}else
	{
		double delta = (newFitness - p.latestFitness) / p.latestFitness;
		if(delta <= 0.01)
		{
			//std::cout << "HA FUNZIONATO \n";
			copyArray(trialPosition, p.position);
			copyArray(trialVelocity, p.velocity);
			p.latestFitness = newFitness;
		}else
		{
			//std::cout << "NON HA FUNZIONATO \n";
		}
	}
}

void Swarm::updateBest(Particle& p)
{
	double newFitness = getFitness(p);
	if(newFitness < p.bestFitness)
	{
		p.bestFitness = newFitness;
		copyArray(p.position, p.bestPosition);
		//std::cout << "New LOCAL best! " << p->bestFitness << "\n";
		if(p.bestFitness < swarmBestFitness)
		{
			swarmBestFitness = p.bestFitness;
			copyArray(p.bestPosition, swarmBest);
			//std::cout << "New GLOBAL best! " << p.bestFitness << " (" << currentIteration << ") \n";
		}
	}
	p.latestFitness = newFitness;
}

void Swarm::initParticle(Particle& p)
{
	p.position = new double[dimension];
	p.bestPosition = new double[dimension];
	p.velocity = new double[dimension];

	p.bestFitness = std::numeric_limits<double>::max( );

	for (int i = 0; i < dimension; i++) 
	{
		p.position[i] = getRandomPosition();
		p.velocity[i] = getRandomVelocity();
	} 

	updateBest(p);
}