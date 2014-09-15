#include "stdafx.h"
#include "Swarm.h"

int _tmain(int argc, _TCHAR* argv[])
{
	bool m = true;
	while(1)
	{
		std::cout << (m ? "mutating \n" : "NOT mutating");
		Swarm swarm;
		swarm.mutate = m;
		swarm.run();

		m = !m;
	}

	return 0;
}

