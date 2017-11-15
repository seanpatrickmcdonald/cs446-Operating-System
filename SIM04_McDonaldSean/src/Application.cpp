#include "Application.h"

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

	Run

**************************************************************
Run through app, will empty vectors
*/

int Application::Run(std::vector<unsigned int> cycleTimeVector)
{    
    //Load all processes and set to ready
	for (unsigned int i = 0; i < processVector.size(); i++)
	{
		processVector[i].controlBlock.processState = READY;
		processQueue.push(processVector[i]);
	}

    std::string string1 = "OS: preparing process ";
    string1.append(processNumber);

    std::string string2 = "OS: starting process ";
    string2.append(processNumber);

	waitOutput(0, string1, string2);

    while (processQueue.empty() == false)
	{
		Process currentProcess = processQueue.front();
		
		//set process to running
		processQueue.front().controlBlock.processState = RUNNING;

		if (currentProcess.processType == P)
		{
            std::string startString = "Process ";
            startString.append(processNumber);
            startString.append(": start processing action");

            std::string endString = "Process ";
            endString.append(processNumber);
            endString.append(": end processing action");


			waitOutput(cycleTimeVector[Processor] * currentProcess.numCycles,
            startString, endString);
		}

		//if it's an input or output
		else if (currentProcess.processType == I || currentProcess.processType == O)
		{
			std::string startString = "Process ";
            startString.append(processNumber);
            startString.append(": start ");

			std::string endString = "Process ";
            endString.append(processNumber);
            endString.append(": end ");

			startString.append(currentProcess.processName);
			endString.append(currentProcess.processName);

			pthread_t threadInt;
			configCommand currentConfig = ConfigHandler::StringToConfigCommand(currentProcess.processName);

			if (currentProcess.processType == I)
			{
				startString.append(" input ");
				endString.append(" input ");
			}

			else if (currentProcess.processType == O)
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
 				currentResourceMutex = &ResourceObject->prtMutexes[resourceNumber];

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

			threadStruct passStruct = {
			startString,
			endString,
			cycleTimeVector[currentConfig] * currentProcess.numCycles,
            currentResourceMutex
			};

			pthread_create(&threadInt, NULL, threadProgram, &passStruct);
			pthread_join(threadInt, NULL);			
		}

		else if (currentProcess.processType == M)
		{
			if (currentProcess.processName == "block")
			{
                std::string startString = "Process ";
                startString.append(processNumber);
                startString.append(": start memory blocking");

                std::string endString = "Process ";
                endString.append(processNumber);
                endString.append(": end memory blocking");

				waitOutput(cycleTimeVector[Memory] * currentProcess.numCycles,
				startString, endString);
			}

			else if (currentProcess.processName == "allocate")
			{
                std::string allocStartString = "Process ";
                allocStartString.append(processNumber);
                allocStartString.append(": allocating memory");

				std::string allocEndString = "Process ";
                allocEndString.append(processNumber);
				allocEndString.append(": memory allocated at 0x");
				allocEndString.append(MemoryObject->AllocateMemory());

				waitOutput(cycleTimeVector[Memory] * currentProcess.numCycles,
				allocStartString, allocEndString);
			}
		}

		processQueue.front().controlBlock.processState = TERMINATED;
		processQueue.pop();
	}

	std::string appEndString;
	appEndString.append(std::to_string(getTime()));
	appEndString.append(" - OS: removing process ");
	appEndString.append(processNumber);

	logHandlerObject->Log(appEndString);

/*
	std::string endOSString;
	endOSString.append(std::to_string(getTime()));
	endOSString.append(" - Simulator program ending");

	logHandlerObject->Log(endOSString);
*/
    return 0;
}




/*************************************************************

	waitOutput

**************************************************************
This function takes in two strings, and prints the time it takes to perform the operation
*/

void Application::waitOutput(unsigned int waitTime, std::string outputString1, std::string outputString2)
{
	std::string startString;
	startString.append(std::to_string(getTime()));
	startString.append(" - ");
	startString.append(outputString1);


	wait(waitTime);


	std::string endString;
	endString.append(std::to_string(getTime()));
	endString.append(" - ");
	endString.append(outputString2);

	logHandlerObject->Log(startString);
	logHandlerObject->Log(endString);
}




/*************************************************************

	threadProgram

**************************************************************
This function is for the I/O thread
*/

void *threadProgram(void* arg)
{
	threadStruct *argStruct = (threadStruct*)arg;
    
	pthread_mutex_lock(argStruct->mutex);
    //std::cout << "Locked mutex: " << argStruct->mutex << std::endl;
	argStruct->app.waitOutput(argStruct->waitTime, argStruct->outputString1, argStruct->outputString2);
    pthread_mutex_unlock(argStruct->mutex);
    //std::cout << "Unlocked mutex: " << argStruct->mutex << std::endl;

	pthread_exit(NULL);
}
