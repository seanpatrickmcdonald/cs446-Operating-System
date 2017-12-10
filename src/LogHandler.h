#ifndef LOG_HANDLER_H
#define LOG_HANDLER_H

#include "SeanBasics.h"

class LogHandler
{
public:
	LogHandler();
	~LogHandler();

	std::string logOutputPath;
	std::vector<std::string> logVector;

	bool logToMonitor;
	bool logToFile;
	
    pthread_mutex_t coutMutex;

	void Log(std::string outputString);
	void PrintLog();
};






#endif
