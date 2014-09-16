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

	bestFitness = std::numeric_limits<double>::max( );

	for (int i = 0; i < motherSwarm->dimension; i++) 
	{
		position[i] = motherSwarm->getRandomPosition();
		velocity[i] = motherSwarm->getRandomVelocity();
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
		if(motherSwarm->crossoverRatio > motherSwarm->getRandomFactor() || !motherSwarm->mutate)
		{
			velocity[i] *= motherSwarm->inertiaWeight;
			velocity[i] += motherSwarm->cognitiveWeight * motherSwarm->getRandomFactor() * ( bestPosition[i] - position[i] );
			velocity[i] += motherSwarm->socialWeight * motherSwarm->getRandomFactor() * ( motherSwarm->swarmBest[i] - position[i] );

			//hybrid part ---------------------------
			if(motherSwarm->mutate)
			{
				int randomParticle1, randomParticle2;

				do
				{
					randomParticle1 = motherSwarm->getRandomIndex();
				}while(randomParticle1 == i);

				do
				{
					randomParticle2 = motherSwarm->getRandomIndex();
				}while(randomParticle2 == i || randomParticle2 == randomParticle1);

				velocity[i] += motherSwarm->mutationWeight * motherSwarm->getRandomFactor() * 
					( motherSwarm->particles[randomParticle1].position[i] - motherSwarm->particles[randomParticle2].position[i]);
			}
			// -------------------------------------

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