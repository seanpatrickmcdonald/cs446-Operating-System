#include "LogHandler.h"

LogHandler::LogHandler()
{
	logToMonitor = false;
	logToFile = false;

    pthread_mutex_init(&coutMutex, NULL);
}

LogHandler::~LogHandler()
{

}

void LogHandler::Log(std::string outputString)
{
	if (logToMonitor)
	{
		pthread_mutex_lock(&coutMutex);
		std::cout.flush() << outputString << std::endl;
		pthread_mutex_unlock(&coutMutex);
	}

	if (logToFile)
	{
		logVector.push_back(outputString);
	}
}

void LogHandler::PrintLog()
{
	if (logToFile)
	{
		std::fstream outputBuffer;
		outputBuffer.open(logOutputPath, std::ios::out | std::ios::app);

		if (outputBuffer)
		{
			outputBuffer << '\n';

			for (int i = 0; i < 50; i++)
			{
				outputBuffer << '*';
			}

			outputBuffer << '\n';

			for (unsigned int i = 0; i < logVector.size(); i++)
			{
				outputBuffer << logVector[i] << std::endl;
			}		

			for (int i = 0; i < 50; i++)
			{
				outputBuffer << '*';
			}

			outputBuffer << "\n";

			outputBuffer.close();
		}
	}
}