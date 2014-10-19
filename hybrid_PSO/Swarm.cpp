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
}

Swarm::~Swarm(void)
{
}

std::string Swarm::toString()
{
	return updateStrategyName;
}

double Swarm::run()
{
	if (functionNumber == 2 || functionNumber == 3 || functionNumber == 4)
		return -1;

	preRun();

	for(int i=0; i<size; i++)
	{
		initParticle(particles[i]);
	}
	for(currentIteration=0; currentIteration<iterations; currentIteration++)
	{
		for(currentParticleIndex=0; currentParticleIndex<size; currentParticleIndex++)
		{
			(this->*updateStrategy)();
		}
	}
	//std::cout << "Final GLOBAL best! " << swarmBestFitness << "\n";
	postRun();
	return swarmBestFitness;
}

void Swarm::preRun()
{
	trialPSOParticlePosition = new double[dimension];
	trialPSOParticleVelocity = new double[dimension];
	trialDEParticle = new double[dimension];
	trialPosition = new double[dimension];
	trialVelocity = new double[dimension];
	particles = new Particle[size];
	fitness = new double;

	swarmBest = new double[dimension];
	currentIteration = -1;
	lastBestFoundAt = -1;
	setNewBest(std::numeric_limits<double>::max());
}

void Swarm::postRun()
{
	delete[] particles;
	delete[] fitness;
	delete[] swarmBest;
	delete[] trialPSOParticlePosition;
	delete[] trialPSOParticleVelocity;
	delete[] trialDEParticle;
}

void Swarm::setNewBest(double d)
{
	swarmBestFitness = d;
	lastBestFoundAt = currentIteration;
}

double Swarm::getFitness()
{
	return getFitness(particles[currentParticleIndex]);
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

void Swarm::updateBest()
{
	updateBest(particles[currentParticleIndex]);
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
			setNewBest(p.bestFitness);
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

//Update Strategies
void Swarm::setUpdateStrategy(UpdateStrategy ups)
{
	switch(ups)
	{
	case(UpdateStrategy::PSO):
		Swarm::updateStrategy = &Swarm::PSO;
		Swarm::updateStrategyName = "PSO";
		break;
	case(UpdateStrategy::HPSO) :
		Swarm::updateStrategy = &Swarm::HPSO;
		Swarm::updateStrategyName = "HPSO";
		break;
	case(UpdateStrategy::HPSOv2) :
		Swarm::updateStrategy = &Swarm::HPSOv2;
		Swarm::updateStrategyName = "HPSOv2";
		break;
	case(UpdateStrategy::HPSOv3) :
		Swarm::updateStrategy = &Swarm::HPSOv3;
		Swarm::updateStrategyName = "HPSOv3";
		break;
	case(UpdateStrategy::HPSORand) :
		Swarm::updateStrategy = &Swarm::HPSORand;
		Swarm::updateStrategyName = "HPSORand";
		break;
	case(UpdateStrategy::DE) :
		Swarm::updateStrategy = &Swarm::DE;
		Swarm::updateStrategyName = "DE-1-Rand";
		break;
	case(UpdateStrategy::HPSONoVel) :
		Swarm::updateStrategy = &Swarm::HPSONoVel;
		Swarm::updateStrategyName = "HPSONoVel";
		break;
	default:
		Swarm::updateStrategy = &Swarm::PSO;
		Swarm::updateStrategyName = "DEFAULT";
		break;
	}
}

void Swarm::PSO()
{
	for (int i = 0; i<dimension; i++)
	{
		particles[currentParticleIndex].velocity[i] *= inertiaWeight;
		particles[currentParticleIndex].velocity[i] += cognitiveWeight * getRandomFactor() * (particles[currentParticleIndex].bestPosition[i] - particles[currentParticleIndex].position[i]);
		particles[currentParticleIndex].velocity[i] += socialWeight * getRandomFactor() * (swarmBest[i] - particles[currentParticleIndex].position[i]);

		if (abs(particles[currentParticleIndex].velocity[i]) > max_velocity)
		{
			particles[currentParticleIndex].velocity[i] = _copysign(max_velocity, particles[currentParticleIndex].velocity[i]);
		}
	}

	for (int i = 0; i<dimension; i++)
	{
		particles[currentParticleIndex].position[i] += particles[currentParticleIndex].velocity[i];
		if (particles[currentParticleIndex].position[i] < min_x)
		{
			particles[currentParticleIndex].position[i] = min_x;
		}
		if (particles[currentParticleIndex].position[i] > max_x)
		{
			particles[currentParticleIndex].position[i] = max_x;
		}
	}
	updateBest(particles[currentParticleIndex]);
}

void Swarm::HPSO()
{
	int randomParticle1, randomParticle2;
	do
	{
		randomParticle1 = getRandomIndex();
	} while (randomParticle1 == currentParticleIndex);

	do
	{
		randomParticle2 = getRandomIndex();
	} while (randomParticle2 == currentParticleIndex || randomParticle2 == randomParticle1);

	int forcedUpdateIndex = getRandomDimensionIndex();
	for (int i = 0; i<dimension; i++)
	{
		if (getRandomFactor() < crossoverRatio || i == forcedUpdateIndex)
		{
			particles[currentParticleIndex].velocity[i] *= inertiaWeight;
			particles[currentParticleIndex].velocity[i] += cognitiveWeight * getRandomFactor() * (particles[currentParticleIndex].bestPosition[i] - particles[currentParticleIndex].position[i]);
			particles[currentParticleIndex].velocity[i] += socialWeight * getRandomFactor() * (swarmBest[i] - particles[currentParticleIndex].position[i]);

			particles[currentParticleIndex].velocity[i] += mutationWeight * getRandomFactor() *
				(particles[randomParticle1].position[i] - particles[randomParticle2].position[i]);
			//((particles[randomParticle1].position[i] - particles[randomParticle2].position[i])/2 - position[i]);

			if (abs(particles[currentParticleIndex].velocity[i]) > max_velocity)
			{
				particles[currentParticleIndex].velocity[i] = _copysign(max_velocity, particles[currentParticleIndex].velocity[i]);
			}
		}
	}

	for (int i = 0; i<dimension; i++)
	{
		particles[currentParticleIndex].position[i] += particles[currentParticleIndex].velocity[i];
		if (particles[currentParticleIndex].position[i] < min_x)
		{
			particles[currentParticleIndex].position[i] = min_x;
		}
		if (particles[currentParticleIndex].position[i] > max_x)
		{
			particles[currentParticleIndex].position[i] = max_x;
		}
	}

	updateBest(particles[currentParticleIndex]);
}

void Swarm::HPSOv2()
{
	copyArray(particles[currentParticleIndex].position, trialPSOParticlePosition);
	copyArray(particles[currentParticleIndex].velocity, trialPSOParticleVelocity);
	copyArray(particles[currentParticleIndex].position, trialDEParticle);

	// PSO
	for (int i = 0; i<dimension; i++)
	{
		trialPSOParticleVelocity[i] *= inertiaWeight;
		trialPSOParticleVelocity[i] += cognitiveWeight * getRandomFactor() * (particles[currentParticleIndex].bestPosition[i] - particles[currentParticleIndex].position[i]);
		trialPSOParticleVelocity[i] += socialWeight * getRandomFactor() * (swarmBest[i] - particles[currentParticleIndex].position[i]);
	}
	for (int i = 0; i<dimension; i++)
	{
		trialPSOParticlePosition[i] += trialPSOParticleVelocity[i];
		if (trialPSOParticlePosition[i] < min_x)
		{
			trialPSOParticlePosition[i] = min_x;
		}
		if (trialPSOParticlePosition[i] > max_x)
		{
			trialPSOParticlePosition[i] = max_x;
		}
	}

	// DE
	int randomParticle1, randomParticle2;
	do
	{
		randomParticle1 = getRandomIndex();
	} while (randomParticle1 == currentParticleIndex);

	do
	{
		randomParticle2 = getRandomIndex();
	} while (randomParticle2 == currentParticleIndex || randomParticle2 == randomParticle1);
	for (int i = 0; i<dimension; i++)
	{
		if (getRandomFactor() < crossoverRatio)
		{
			trialDEParticle[i] += getRandomFactor() * mutationWeight * (particles[randomParticle1].position[i] - particles[randomParticle2].position[i]);
		}
	}

	// Cross-Over
	for (int i = 0; i<dimension; i++)
	{
		double r = getRandomFactor();
		double oldPosition = particles[currentParticleIndex].position[i];

		if (r < 0.8)
		{
			particles[currentParticleIndex].position[i] = trialPSOParticlePosition[i];
		}
		else
		{
			particles[currentParticleIndex].position[i] = trialDEParticle[i];
		}

		particles[currentParticleIndex].velocity[i] = particles[currentParticleIndex].position[i] - oldPosition;
	}
	updateBest(particles[currentParticleIndex]);
}

void Swarm::HPSOv3()
{
	copyArray(particles[currentParticleIndex].position, trialPSOParticlePosition);
	copyArray(particles[currentParticleIndex].velocity, trialPSOParticleVelocity);
	copyArray(particles[currentParticleIndex].position, trialDEParticle);

	// PSO
	for(int i=0; i<dimension; i++)
	{
		trialPSOParticleVelocity[i] *= inertiaWeight;
		trialPSOParticleVelocity[i] += cognitiveWeight * getRandomFactor() * ( particles[currentParticleIndex].bestPosition[i] - particles[currentParticleIndex].position[i] );
		trialPSOParticleVelocity[i] += socialWeight * getRandomFactor() * ( swarmBest[i] - particles[currentParticleIndex].position[i] );
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
	}while(randomParticle1 == currentParticleIndex);

	do
	{
		randomParticle2 = getRandomIndex();
	}while(randomParticle2 == currentParticleIndex || randomParticle2 == randomParticle1);
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
		double oldPosition = particles[currentParticleIndex].position[i];

		if(r < 0.9)
		{
			trialPosition[i] = trialPSOParticlePosition[i];
		}else 
		{
			trialPosition[i] = trialDEParticle[i];
		}

		trialVelocity[i] = particles[currentParticleIndex].position[i] - oldPosition;
	}
	
	// Selection
	double newFitness = getFitness(trialPosition);
	if(newFitness < particles[currentParticleIndex].bestFitness)
	{
		particles[currentParticleIndex].bestFitness = newFitness;
		copyArray(particles[currentParticleIndex].position, particles[currentParticleIndex].bestPosition);
		//std::cout << "New LOCAL best! " << p->bestFitness << "\n";
		if(particles[currentParticleIndex].bestFitness < swarmBestFitness)
		{
			setNewBest(particles[currentParticleIndex].bestFitness);
			copyArray(particles[currentParticleIndex].bestPosition, swarmBest);
			//std::cout << "New GLOBAL best! " << p.bestFitness << " (" << currentIteration << ") \n";
		}
		copyArray(trialPosition, particles[currentParticleIndex].position);
		copyArray(trialVelocity, particles[currentParticleIndex].velocity);
		particles[currentParticleIndex].latestFitness = newFitness;
	}else
	{
		double delta = (newFitness - particles[currentParticleIndex].latestFitness) / particles[currentParticleIndex].latestFitness;
		if(delta <= 0.01)
		{
			//std::cout << "HA FUNZIONATO \n";
			copyArray(trialPosition, particles[currentParticleIndex].position);
			copyArray(trialVelocity, particles[currentParticleIndex].velocity);
			particles[currentParticleIndex].latestFitness = newFitness;
		}else
		{
			//std::cout << "NON HA FUNZIONATO \n";
		}
	}
}

void Swarm::HPSORand()
{
	if (getRandomFactor() < 0.1)
	{
		{
			copyArray(particles[currentParticleIndex].position, trialPSOParticlePosition);
			copyArray(particles[currentParticleIndex].velocity, trialPSOParticleVelocity);

			double oldIntertiaWeight = particles[currentParticleIndex]._inertiaWeight;
			double oldCognitiveWeight = particles[currentParticleIndex]._cognitiveWeight;
			double oldSocialWeight = particles[currentParticleIndex]._socialWeight;
			double oldMutationWeight = particles[currentParticleIndex]._mutationWeight;

			reweight();

			int randomParticle1, randomParticle2;
			do
			{
				randomParticle1 = getRandomIndex();
			} while (randomParticle1 == currentParticleIndex);

			do
			{
				randomParticle2 = getRandomIndex();
			} while (randomParticle2 == currentParticleIndex || randomParticle2 == randomParticle1);

			int forcedUpdateIndex = getRandomDimensionIndex();
			for (int i = 0; i<dimension; i++)
			{
				if (getRandomFactor() < crossoverRatio || i == forcedUpdateIndex)
				{
					particles[currentParticleIndex].velocity[i] *= particles[currentParticleIndex]._inertiaWeight;
					particles[currentParticleIndex].velocity[i] += particles[currentParticleIndex]._cognitiveWeight * getRandomFactor() * (particles[currentParticleIndex].bestPosition[i] - particles[currentParticleIndex].position[i]);
					particles[currentParticleIndex].velocity[i] += particles[currentParticleIndex]._socialWeight * getRandomFactor() * (swarmBest[i] - particles[currentParticleIndex].position[i]);
					particles[currentParticleIndex].velocity[i] += particles[currentParticleIndex]._mutationWeight * getRandomFactor() * (particles[randomParticle1].position[i] - particles[randomParticle2].position[i]);

					if (abs(particles[currentParticleIndex].velocity[i]) > max_velocity)
					{
						particles[currentParticleIndex].velocity[i] = _copysign(max_velocity, particles[currentParticleIndex].velocity[i]);
					}
				}
			}

			for (int i = 0; i<dimension; i++)
			{
				particles[currentParticleIndex].position[i] += particles[currentParticleIndex].velocity[i];
				if (particles[currentParticleIndex].position[i] < min_x)
				{
					particles[currentParticleIndex].position[i] = min_x;
				}
				if (particles[currentParticleIndex].position[i] > max_x)
				{
					particles[currentParticleIndex].position[i] = max_x;
				}
			}

			double newFitness = getFitness();
			if (newFitness < particles[currentParticleIndex].bestFitness)
			{
				particles[currentParticleIndex].bestFitness = newFitness;
				copyArray(particles[currentParticleIndex].position, particles[currentParticleIndex].bestPosition);
				//std::cout << "New LOCAL best! " << p->bestFitness << "\n";
				if (particles[currentParticleIndex].bestFitness < swarmBestFitness)
				{
					setNewBest(particles[currentParticleIndex].bestFitness);
					copyArray(particles[currentParticleIndex].bestPosition, swarmBest);
					//std::cout << "New GLOBAL best! " << p.bestFitness << " (" << currentIteration << ") \n";
				}
				particles[currentParticleIndex].latestFitness = newFitness;
			}
			else 
			{
				// revert the changes
				copyArray(trialPSOParticlePosition, particles[currentParticleIndex].position);
				copyArray(trialPSOParticleVelocity, particles[currentParticleIndex].velocity);

				particles[currentParticleIndex]._inertiaWeight = oldIntertiaWeight;
				particles[currentParticleIndex]._cognitiveWeight = oldCognitiveWeight;
				particles[currentParticleIndex]._socialWeight = oldSocialWeight;
				particles[currentParticleIndex]._mutationWeight = oldMutationWeight;
			}
		}
	}
	else
	{
		HPSO();
	}
}

void Swarm::reweight()
{
	particles[currentParticleIndex]._cognitiveWeight = getRandomFactor();
	particles[currentParticleIndex]._socialWeight = getRandomFactor();
	particles[currentParticleIndex]._mutationWeight = getRandomFactor() / 100;

	double scalingFactor = 4.1 / (particles[currentParticleIndex]._cognitiveWeight + particles[currentParticleIndex]._socialWeight + particles[currentParticleIndex]._mutationWeight);
	particles[currentParticleIndex]._cognitiveWeight *= scalingFactor;
	particles[currentParticleIndex]._socialWeight *= scalingFactor;
	particles[currentParticleIndex]._mutationWeight *= scalingFactor;
}

void Swarm::DE()
{
	int randomParticle1, randomParticle2, randomParticle3;
	randomParticle1 = getRandomIndex();
	do
	{
		randomParticle2 = getRandomIndex();
	} while (randomParticle2 == randomParticle1);

	do
	{
		randomParticle3 = getRandomIndex();
	} while (randomParticle3 == randomParticle1 || randomParticle3 == randomParticle2);

	int forcedCrossoverIndex = getRandomDimensionIndex();
	for (int i = 0; i < dimension; i++)
	{
		if (getRandomFactor() < crossoverRatio || i == forcedCrossoverIndex)
		{
			trialDEParticle[i] = particles[randomParticle1].position[i] + 0.5 * (particles[randomParticle2].position[i] - particles[randomParticle3].position[i]);
		}
		else
		{
			trialDEParticle[i] = particles[randomParticle1].position[i];
		}
	}

	double newFitness = getFitness(trialDEParticle);
	if (newFitness < particles[randomParticle1].bestFitness)
	{
		copyArray(trialDEParticle, particles[randomParticle1].position);
		particles[randomParticle1].bestFitness = newFitness;

		if (newFitness < swarmBestFitness)
		{
			setNewBest(newFitness);
		}
	}
}

void Swarm::HPSONoVel()
{
	int randomParticle1, randomParticle2;
	do
	{
		randomParticle1 = getRandomIndex();
	} while (randomParticle1 == currentParticleIndex);

	do
	{
		randomParticle2 = getRandomIndex();
	} while (randomParticle2 == currentParticleIndex || randomParticle2 == randomParticle1);

	int forcedUpdateIndex = getRandomDimensionIndex();
	for (int i = 0; i<dimension; i++)
	{
		if (getRandomFactor() < crossoverRatio || i == forcedUpdateIndex)
		{
			trialPSOParticleVelocity[i] += cognitiveWeight * getRandomFactor() * (particles[currentParticleIndex].bestPosition[i] - particles[currentParticleIndex].position[i]);
			trialPSOParticleVelocity[i] += socialWeight * getRandomFactor() * (swarmBest[i] - particles[currentParticleIndex].position[i]);
			trialPSOParticleVelocity[i] += mutationWeight * getRandomFactor() * (particles[randomParticle1].position[i] - particles[randomParticle2].position[i]);

			if (abs(particles[currentParticleIndex].velocity[i]) > max_velocity)
			{
				trialPSOParticleVelocity[i] = _copysign(max_velocity, trialPSOParticleVelocity[i]);
			}
		}
	}

	for (int i = 0; i<dimension; i++)
	{
		trialPSOParticlePosition[i] += trialPSOParticleVelocity[i];
		if (trialPSOParticlePosition[i] < min_x)
		{
			trialPSOParticlePosition[i] = min_x;
		}
		if (trialPSOParticlePosition[i] > max_x)
		{
			trialPSOParticlePosition[i] = max_x;
		}
	}

	double newFitness = getFitness(trialPSOParticlePosition);
	if (newFitness < particles[currentParticleIndex].bestFitness)
	{
		copyArray(trialPSOParticlePosition, particles[currentParticleIndex].position);
		copyArray(trialPSOParticlePosition, particles[currentParticleIndex].bestPosition);
		particles[currentParticleIndex].bestFitness = newFitness;

		if (particles[currentParticleIndex].bestFitness < swarmBestFitness)
		{
			setNewBest(particles[currentParticleIndex].bestFitness);
			copyArray(particles[currentParticleIndex].bestPosition, swarmBest);
		}
	}
}