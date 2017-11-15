#include "Scheduler.h"


void sortSJF(std::vector<Application> &apps)
{
	std::vector<Application> copyVector = apps;

	if (apps.size() > 0)
		apps.clear();

	while(copyVector.size() > 0)
	{
		unsigned int least_tasks = 999;
		unsigned int least_index = 999;

		for (unsigned int i = 0; i < copyVector.size(); i++)
		{
			if (copyVector[i].num_tasks < least_tasks)
			{
				least_tasks = copyVector[i].num_tasks;
				least_index = i;
			}
		}

		apps.push_back(copyVector[least_index]);
		copyVector.erase(copyVector.begin() + least_index);
	}
}
void sortPS(std::vector<Application> &apps)
{
	std::vector<Application> copyVector = apps;

	if (apps.size() > 0)
		apps.clear();

	while(copyVector.size() > 0)
	{
		unsigned int most_io_tasks = 0;
		unsigned int most_io_index = 0;

		for (unsigned int i = 0; i < copyVector.size(); i++)
		{
			if (copyVector[i].num_io_tasks > most_io_tasks)
			{
				most_io_tasks = copyVector[i].num_io_tasks;
				most_io_index = i;
			}
		}

		apps.push_back(copyVector[most_io_index]);
		copyVector.erase(copyVector.begin() + most_io_index);
	}
}