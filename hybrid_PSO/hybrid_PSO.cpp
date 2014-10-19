#include "stdafx.h"
#include "Swarm.h"
#include <windows.h>
#include <ctime>
#include <thread>

#define TEST_NUMBER (100)

void runBenchmarkTests(UpdateStrategy, const int, const bool);

int main(int argc, char* argv[])
{	
	if (argc == 1)
	{
		std::cout << "Usage: " << argv[0] << " algorithm_name" << std::endl;
		exit(1);
	}

	std::cout << argv[1] << std::endl;

	if (!strcmp(argv[1], "pso"))
	{
		runBenchmarkTests(PSO, TEST_NUMBER, true);
	}
	else if (!strcmp(argv[1], "hpso"))
	{
		runBenchmarkTests(HPSO, TEST_NUMBER, true);
	}
	else if (!strcmp(argv[1], "hpsov2"))
	{
		runBenchmarkTests(HPSOv2, TEST_NUMBER, true);
	}
	else if (!strcmp(argv[1], "hpsov3"))
	{
		runBenchmarkTests(HPSOv3, TEST_NUMBER, true);
	}
	else if (!strcmp(argv[1], "hpsorand"))
	{
		runBenchmarkTests(HPSORand, TEST_NUMBER, true);
	}
	else 
	{
		std::cout << "Invalid argument." << std::endl;
	}

	//std::thread tPSO(runBenchmarkTests, PSO, 1, true);
	//std::thread tHPSO(runBenchmarkTests, HPSO, 1, true);
	//std::thread tHPSOv2(runBenchmarkTests, HPSOv2, 100, true);
	//std::thread tHPSOv3(runBenchmarkTests, HPSOv3, 100, true);
	//std::thread tHPSORand(runBenchmarkTests, HPSORand, 100, true);

	//tPSO.join();
	//tHPSO.join();
	//tHPSOv2.join();
	//tHPSOv3.join();
	//tHPSORand.join();

	return 0;
}

void runBenchmarkTests(UpdateStrategy ups, const int maxIter, const bool loop)
{
	do
	{
		Swarm swarm;
		swarm.setUpdateStrategy(ups);

		int iter = 0;
		int const maxFunc = 28;

		double** results = new double*[maxIter];
		for (int i = 0; i < maxIter; ++i)
		{
			results[i] = new double[maxFunc]{-1};
		}

		for (int func = 0; func < maxFunc; func++)
		{
			
			for (int i = 0; i < maxIter; i++)
			{
				system("CLS");
				std::cout << swarm.toString() << ": function " << (func + 1) << " of " << maxFunc << ",\t iteration " << (i + 1) << " of " << maxIter << std::endl;

				swarm.functionNumber = func + 1;
				results[i][func] = swarm.run();
			}
		}

		// log
		std::time_t rawtime;
		std::tm* timeinfo;
		char buffer[80];

		std::time(&rawtime);
		timeinfo = std::localtime(&rawtime);
		std::strftime(buffer, 80, "_%d%m%y-%H%M%S", timeinfo);
		std::puts(buffer);

		std::ofstream log;
		std::string filename = "LOG_", directory = "c:/logs/";
		filename += swarm.toString();
		filename += buffer;
		filename += "_" + std::to_string(static_cast<unsigned long long>(maxIter));
		filename += ".txt";
		log.open(directory + filename);

		for (int func = 0; func < maxFunc; func++)
		{
			for (int i = 0; i < maxIter; i++)
			{
				log << results[i][func] << "\t";
			}
			log << std::endl;
		}
		log.close();
	} while (loop);
}