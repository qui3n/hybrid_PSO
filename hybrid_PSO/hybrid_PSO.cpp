#include "stdafx.h"
#include "Swarm.h"

int _tmain(int argc, _TCHAR* argv[])
{
	Swarm swarm;

	int num = 0;
	double bestFitness[28];
	double sumFitness = 0;
	int i = 1;

	while(num <= 27)
	{
		bestFitness[num] = swarm.swarmBestFitness;
		num++;
		sumFitness += swarm.swarmBestFitness;
		double average = sumFitness/i;
		std::cout << "Average: " << average << "\n";
		i++;
	}
	for(int i=0; i<28; i++)
	{
		std::cout << "function " << i+1 << ": \t" << bestFitness[i] << "\n";
	}
	while(1);
	return 0;
}

