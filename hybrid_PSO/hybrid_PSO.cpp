#include "stdafx.h"
#include "Swarm.h"
#include <windows.h>

int _tmain(int argc, _TCHAR* argv[])
{		
	Swarm swarm;
	int iter = 1;
	double bestFitness[28] = {0};
	int successNumber[28] = {0};

	std::ofstream log;
	log.open("log.txt");

	for(int k=0; k<1; k++)
	{
		int num = 0;
		while(num <= 27)
		{
			swarm.functionNumber = num+1;
			swarm.run();
			//if(swarm.swarmBestFitness < 200)
			{
				bestFitness[num] += swarm.swarmBestFitness;
				successNumber[num]++;
			}
			num++;
		}
		system("CLS");
		std::cout << "iteration " << iter << ":\tsuccess\taverage \n";
		for(int i=0; i<28; i++)
		{
			std::cout << "function " << i+1 << ": \t(" << successNumber[i] << ")\t" << ( successNumber[i] != 0 ? (bestFitness[i] / successNumber[i]) : -1 ) << "\n";
		}

		iter++;
	}

	log << "iteration " << iter << ":\tsuccess\taverage \n";
	for(int i=0; i<28; i++)
	{
		log << "function " << i+1 << ": \t" << successNumber[i] << "\t" << ( successNumber[i] != 0 ? (bestFitness[i] / successNumber[i]) : -1 ) << "\n";
	}
	log.close();

	return 0;
}