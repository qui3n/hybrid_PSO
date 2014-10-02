#include "stdafx.h"
#include "Swarm.h"

int _tmain(int argc, _TCHAR* argv[])
{
	Swarm swarm;
	double sumFitness=0;
	int i=1;

	while(1)
	{
		swarm.run();
		sumFitness += swarm.swarmBestFitness;
		double average = sumFitness/i;
		std::cout << "Average: " << average << "\n";
		i++;
	}
	return 0;
}

