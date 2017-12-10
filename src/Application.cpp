#include "Application.h"

int Application::quantum = 0;

Application::Application()
{
    
}

Application::Application(int processNum, LogHandler *logHandlerIn, MemoryHandler *MemoryObjectIn, ResourceHandler *ResourceObjectIn)
{
    processNumber = IntToString(processNum);

    logHandlerObject = logHandlerIn;
    MemoryObject = MemoryObjectIn;
    ResourceObject = ResourceObjectIn;
}

Application::~Application()
{

}

/*************************************************************

	Initalize

**************************************************************
Initalize the queue of the app
*/

void Application::Initialize(std::vector<unsigned int> cycleTimeVector)
{
    //Load all processes and set to ready
	for (unsigned int i = 0; i < processVector.size(); i++)
	{
		processVector[i].controlBlock.processState = READY;
		processQueue.push(processVector[i]);
	}
}

/*************************************************************

	Run

**************************************************************
Run through app
*/

int Application::Run(std::vector<unsigned int> cycleTimeVector)
{    
	int timeLeft = Application::quantum;

    std::string string1 = "OS: preparing process ";
    string1.append(processNumber);

    std::string string2 = "OS: starting process ";
    string2.append(processNumber);

	waitOutput(0, string1, string2);

    while (processQueue.empty() == false)
	{
		Process *currentProcess = &processQueue.front();
		
		//set process to running
		processQueue.front().controlBlock.processState = RUNNING;


        std::string startString = "Process ";
        startString.append(processNumber);
        startString.append(": ");

        std::string endString = "Process ";
        endString.append(processNumber);
        endString.append(": ");

        std::string interruptString = "Process ";
        interruptString.append(processNumber);
        interruptString.append(": ");

		if (currentProcess->processType == P)
		{
            startString.append("start processing action");

            endString.append("end processing action");

            interruptString.append("interrupted processing action");
		}

		else if (currentProcess->processType == M)
		{
			if (currentProcess->processName == "block")
			{
                startString.append("memory blocking");

                endString.append("memory blocking");

            	interruptString.append("interrupted memory blocking");
			}

			else if (currentProcess->processName == "allocate")
			{
                startString.append("allocating memory");

				endString.append("memory allocated at 0x");
				endString.append(MemoryObject->AllocateMemory());

            	interruptString.append("interrupted allocating memory");
			}
		}

		//if it's an input or output
		else if (currentProcess->processType == I || currentProcess->processType == O)
		{
			startString.append("start ");
			startString.append(currentProcess->processName);

            endString.append("end ");
			endString.append(currentProcess->processName);


			configCommand currentConfig = ConfigHandler::StringToConfigCommand(currentProcess->processName);

			if (currentProcess->processType == I)
			{
				startString.append(" input ");
				endString.append(" input ");
			}

			else if (currentProcess->processType == O)
			{
				startString.append(" output ");
				endString.append(" output ");
			}

			//Getting Mutex Information
			pthread_mutex_t *currentResourceMutex;
            unsigned int resourceNumber;
            if (currentConfig == Printer)
            {
                resourceNumber = ResourceObject->GetAvailablePrinter();
                currentResourceMutex = &ResourceObject->prtMutexes[resourceNumber];

				startString.append("on PRNTR ");
                startString.append(IntToString(resourceNumber));
				endString.append("on PRNTR ");
                endString.append(IntToString(resourceNumber));

                //std::cout << "Printer number " << resourceNumber << " with mutex: " << currentResourceMutex << std::endl;
            }
			else if (currentConfig == HardDrive)
			{
                resourceNumber = ResourceObject->GetAvailableHdd();
 				currentResourceMutex = &ResourceObject->hddMutexes[resourceNumber];

				startString.append("on HDD ");
                startString.append(IntToString(resourceNumber));
				endString.append("on HDD ");
                endString.append(IntToString(resourceNumber));

                //std::cout << "Hdd number " << resourceNumber << " with mutex: " << currentResourceMutex << std::endl;
			}
			else if (currentConfig == Keyboard)
			{
 				currentResourceMutex = ResourceObject->kybMutex;

                //std::cout << "Keyboard mutex: " << currentResourceMutex << std::endl;
			}
			else if (currentConfig == Monitor)
			{
 				currentResourceMutex = ResourceObject->monMutex;

                //std::cout << "Monitor mutex: " << currentResourceMutex << std::endl;
			}

			threadStruct *passStruct = new threadStruct(); 

			passStruct->outputString1 = startString;
			passStruct->outputString2 = endString;
			passStruct->waitTime = currentProcess->runTime;
            passStruct->mutex = currentResourceMutex;
            passStruct->app = this;
			

    		pthread_t threadInt;
			pthread_create(&threadInt, NULL, threadProgramIO, passStruct);
			IOthreads.push_back(threadInt);

			processQueue.front().controlBlock.processState = TERMINATED;
			processQueue.pop();		

			num_io_tasks--, num_tasks--;
		}

		//Run quantum time, 
		if (currentProcess->processType != I && currentProcess->processType != O)
		{
			int timeRun = waitOutput(currentProcess->runTime, startString, endString, timeLeft, interruptString);

			currentProcess->runTime -= timeRun;
			timeLeft -= timeRun;

			if (currentProcess->runTime <= 0)
			{
				processQueue.front().controlBlock.processState = TERMINATED;
				processQueue.pop();
				num_tasks--;
			}

			if (timeLeft <= 0)
			{
				return -1;
			}
		}
	}

	std::string appEndString;
	appEndString.append(std::to_string(getTime()));
	appEndString.append(" - OS: Process ");
	appEndString.append(processNumber);
	appEndString.append(" completed.");

	logHandlerObject->Log(appEndString);

    return 0;
}




/*************************************************************

	waitOutput

**************************************************************
This function takes in two strings, and prints the time it takes to perform the operation
*/

int Application::waitOutput(unsigned int waitTime, std::string outputString1, std::string outputString2, int max,
	std::string intString)
{
	std::string startString;
	startString.append(std::to_string(getTime()));
	startString.append(" - ");
	startString.append(outputString1);

	logHandlerObject->Log(startString);

	if (max >= 0)
	{
		if ((int)waitTime <= max)
		{
			wait(waitTime);
		}

		else
		{
			wait(max);

			std::string endString;
			endString.append(std::to_string(getTime()));
			endString.append(" - ");
			endString.append(intString);

			logHandlerObject->Log(endString);

			return max;
		}
	}

	else
	{
		wait(waitTime);
	}


	std::string endString;
	endString.append(std::to_string(getTime()));
	endString.append(" - ");
	endString.append(outputString2);

	logHandlerObject->Log(endString);

	return (int)waitTime;
}




/*************************************************************

	threadProgramIO

**************************************************************
This function is for the I/O thread
*/

void *threadProgramIO(void* arg)
{
	threadStruct *argStruct = (threadStruct*)arg;
    
	pthread_mutex_lock(argStruct->mutex);
    //std::cout << "Locked mutex: " << argStruct->mutex << std::endl;
	argStruct->app->waitOutput(argStruct->waitTime, argStruct->outputString1, argStruct->outputString2);
    pthread_mutex_unlock(argStruct->mutex);
    //std::cout << "Unlocked mutex: " << argStruct->mutex << std::endl;

	pthread_exit(NULL);
}
