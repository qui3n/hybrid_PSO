#include "stdafx.h"
#include "Swarm.h"
#include <windows.h>
#include <ctime>


#define DEFAULT_TEST_NUMBER (1)

void runBenchmarkTests(UpdateStrategy, const int, const bool);

//int main(int argc, char* argv[])
//{	
//	int test_number = DEFAULT_TEST_NUMBER;
//	bool loop = true;
//	if (argc == 1)
//	{
//		std::cout << "Usage: " << argv[0] << " algorithm_name" << std::endl;
//		exit(1);
//	} 
//	else
//	{
//		if (argc > 2)
//		{
//			if (atoi(argv[2]) > 0)
//			{
//				test_number = atoi(argv[2]);
//			}
//		}
//	}
//
//	if (!strcmp(argv[1], "pso"))
//	{
//		runBenchmarkTests(PSO, test_number, loop);
//	}
//	else if (!strcmp(argv[1], "hpso"))
//	{
//		runBenchmarkTests(HPSO, test_number, loop);
//	}
//	else if (!strcmp(argv[1], "hpsov2"))
//	{
//		runBenchmarkTests(HPSOv2, test_number, loop);
//	}
//	else if (!strcmp(argv[1], "hpsov3"))
//	{
//		runBenchmarkTests(HPSOv3, test_number, loop);
//	}
//	else if (!strcmp(argv[1], "hpsorand"))
//	{
//		runBenchmarkTests(HPSORand, test_number, loop);
//	}
//	else if (!strcmp(argv[1], "de"))
//	{
//		runBenchmarkTests(DE, test_number, loop);
//	}
//	else if (!strcmp(argv[1], "hpsonovel"))
//	{
//		runBenchmarkTests(HPSONoVel, test_number, loop);
//	}
//	else 
//	{
//		std::cout << "Invalid argument." << std::endl;
//	}
//	return 0;
//}

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
		double** conv = new double*[maxIter];
		for (int i = 0; i < maxIter; ++i)
		{
			conv[i] = new double[maxFunc]{-1};
		}

		for (int func = 0; func < maxFunc; func++)
		{
			
			for (int i = 0; i < maxIter; i++)
			{
				system("CLS");
				std::cout << swarm.toString() << ": function " << (func + 1) << " of " << maxFunc << ",\t iteration " << (i + 1) << " of " << maxIter << std::endl;

				swarm.functionNumber = func + 1;
				results[i][func] = swarm.run();
				conv[i][func] = swarm.lastBestFoundAt;
			}
		}

		// log
		std::time_t rawtime;
		std::tm* timeinfo;
		char buffer[80];

		std::time(&rawtime);
		timeinfo = std::localtime(&rawtime);
		std::strftime(buffer, 80, "_%y%m%d-%H%M%S", timeinfo);
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
		log << std::endl;
		for (int func = 0; func < maxFunc; func++)
		{
			for (int i = 0; i < maxIter; i++)
			{
				log << conv[i][func] << "\t";
			}
			log << std::endl;
		}
		log.close();
	} while (loop);
}