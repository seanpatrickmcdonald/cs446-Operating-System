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

    void Initialize(std::vector<unsigned int>);
    int Run(std::vector<unsigned int>);

    LogHandler *logHandlerObject;
    MemoryHandler *MemoryObject;
    ResourceHandler *ResourceObject;

    int waitOutput(unsigned int waitTime, std::string outputString1, std::string outputString2, int max = -1
        , std::string intString = "asdf");

    std::vector<Process> processVector;
    std::queue<Process> processQueue;

    static int quantum;

    std::string processNumber;
    unsigned int num_io_tasks, num_tasks;


    std::vector<pthread_t> IOthreads;
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
    Application *app;
};

/*************************************************************

	threadProgram

**************************************************************
This function is for the I/O thread
*/

void *threadProgramIO(void* arg);





#endif
