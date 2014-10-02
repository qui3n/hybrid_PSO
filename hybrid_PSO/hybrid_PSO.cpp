#include "stdafx.h"
#include "Swarm.h"

int _tmain(int argc, _TCHAR* argv[])
{
	Swarm swarm;
	int num = 0;
	double bestFitness[28];
	while(num <= 27)
	{
		swarm.functionNumber = num+1;
		swarm.run();
		bestFitness[num] = swarm.swarmBestFitness;
		num++;
	}
	for(int i=0; i<28; i++){
		std::cout << "function " << i+1 << ": \t" << bestFitness[i] << "\n";
	}
	while(1);
	return 0;
}

