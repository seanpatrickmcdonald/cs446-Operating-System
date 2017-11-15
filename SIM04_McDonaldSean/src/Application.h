#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "SeanBasics.h"
#include "LogHandler.h"
#include "MemoryHandler.h"
#include "ResourceHandler.h"
#include "ConfigHandler.h"
#include "Process.h"

#define CHRONO_RESOLUTION std::chrono::microseconds

class Application
{
public:
    Application();
    Application(int processNum, LogHandler *logHandlerIn, MemoryHandler *MemoryObjectIn, ResourceHandler *ResourceObjectIn);
    ~Application();

    int Run(std::vector<unsigned int>);

    LogHandler *logHandlerObject;
    MemoryHandler *MemoryObject;
    ResourceHandler *ResourceObject;

    void waitOutput(unsigned int waitTime, std::string outputString1, std::string outputString2);

    std::vector<Process> processVector;
    std::queue<Process> processQueue;

    std::string processNumber;
    unsigned int num_io_tasks, num_tasks;
};

/*************************************************************

	ThreadStruct

**************************************************************
This struct is the argument passed to the threadProgram
*/

struct threadStruct
{
	std::string outputString1;
	std::string outputString2;
	unsigned int waitTime;	
    pthread_mutex_t *mutex;
    Application app;
};

/*************************************************************

	threadProgram

**************************************************************
This function is for the I/O thread
*/

void *threadProgram(void* arg);





#endif
