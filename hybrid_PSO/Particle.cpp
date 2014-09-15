#include "Particle.h"
#include <iostream>
#include <math.h>
#include <random>

Particle::Particle()
{
}


Particle::~Particle(void)
{
}

void Particle::init(Swarm* swarm)
{
	motherSwarm = swarm;

	position = new double[motherSwarm->dimension];
	bestPosition = new double[motherSwarm->dimension];
	velocity = new double[motherSwarm->dimension];

	bestFitness = 77777; // TODO: max_double

	std::random_device random;
    std::mt19937 gen(random());
	std::uniform_real_distribution<double> pos_dis(motherSwarm->min_x, motherSwarm->max_x);
	std::uniform_real_distribution<double> vel_dis(-motherSwarm->max_velocity, motherSwarm->max_velocity);

	for (int i = 0; i < motherSwarm->dimension; i++) 
	{
        position[i] = pos_dis(gen);
		velocity[i] = vel_dis(gen);
		//std::cout << position[i] << " " << velocity[i] << "\n";
    } 

	checkBest();
}

void Particle::copyArray(double* src, double* dest)
{
	for(int i = 0; i < motherSwarm->dimension; i++)
	{
		dest[i] = src[i];
	}
}

void Particle::update()
{
	for(int i=0; i<motherSwarm->dimension; i++)
	{
		if(motherSwarm->crossoverRatio > getRandomFactor())
		{
			velocity[i] *= motherSwarm->inertiaWeight;
			velocity[i] += motherSwarm->cognitiveWeight * getRandomFactor() * ( bestPosition[i] - position[i] );
			velocity[i] += motherSwarm->socialWeight * getRandomFactor() * ( motherSwarm->swarmBest[i] - position[i] );

			if(abs(velocity[i]) > motherSwarm->max_velocity)
			{
				velocity[i] = _copysign(motherSwarm->max_velocity, velocity[i]);
			}
		}
		
	}

	for(int i=0; i<motherSwarm->dimension; i++)
	{
		position[i] += velocity[i];
		if(position[i] < motherSwarm->min_x)
		{
			position[i] = motherSwarm->min_x;
		}
		if(position[i] > motherSwarm->max_x)
		{
			position[i] = motherSwarm->max_x;
		}
	}

	checkBest();
}

double Particle::getRandomFactor()
{
	std::random_device random;
    std::mt19937 gen(random());
	std::uniform_real_distribution<double> dis(0, 1);

	return dis(gen);
}

void Particle::checkBest()
{
	double newFitness = motherSwarm->getFitness(this);
	if(newFitness < bestFitness)
	{
		bestFitness = newFitness;
		copyArray(position, bestPosition);
		//std::cout << "New LOCAL best! " << bestFitness << "\n";
		if(bestFitness < motherSwarm->swarmBestFitness)
		{
			motherSwarm->swarmBestFitness = bestFitness;
			copyArray(bestPosition, motherSwarm->swarmBest);
			std::cout << "New GLOBAL best! " << bestFitness << " (" << motherSwarm->currentIteration << ") \n";
		}
	}
}