#include "stdafx.h"
#include "Swarm.h"
#include <windows.h>
#include <ctime>
#include <string>

int _tmain(int argc, _TCHAR* argv[])
{		
	Swarm swarm;
	int iter = 0;
	int const maxIter = 10;
	double bestFitness[28] = {0};
	int successNumber[28] = {0};

	for(int k=0; k<maxIter; k++)
	{
		iter++;
		for(int num=0; num<28; num++)
		{
			swarm.functionNumber = num+1;
			swarm.run();
			//if(swarm.swarmBestFitness < 200)
			{
				bestFitness[num] += swarm.swarmBestFitness;
				successNumber[num]++;
			}
		}
		system("CLS");
		std::cout << "iteration " << iter << ":\tsuccess\taverage \n";
		for(int i=0; i<28; i++)
		{
			std::cout << "function " << i+1 << ": \t(" << successNumber[i] << ")\t" << ( successNumber[i] != 0 ? (bestFitness[i] / successNumber[i]) : -1 ) << "\n";
		}
	}

	// log
	std::time_t rawtime;
	std::tm* timeinfo;
	char buffer [80];

	std::time(&rawtime);
	timeinfo = std::localtime(&rawtime);
	std::strftime(buffer,80,"%Y_%m_%d-%H_%M_%S",timeinfo);
	std::puts(buffer);

	std::ofstream log;
	std::string filename = "logs_", directory = "c:/logs/";
	filename += buffer;
	filename += "_HPSOv2_";
	filename += std::to_string(static_cast<unsigned long long>(maxIter));
	filename += ".txt";
	log.open(directory + filename);

	log << "iteration " << iter << ":\tsuccess\taverage \n";
	for(int i=0; i<28; i++)
	{
		log << "function " << i+1 << ": \t" << successNumber[i] << "\t" << ( successNumber[i] != 0 ? (bestFitness[i] / successNumber[i]) : -1 ) << "\n";
	}
	log.close();

	return 0;
}