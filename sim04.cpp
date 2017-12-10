#ifndef _MAIN_CPP
#define _MAIN_CPP


#include "SeanOS.h"

std::string *configCommandArray;
void InitializeGlobals();

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Proper usage is: \"./Sim04 [config filename]\"" << std::endl;
		return -1;
	}

    InitializeGlobals();   

    
    LogHandler *logHandlerObject = new LogHandler;
    MemoryHandler *MemoryObject = new MemoryHandler;
    ResourceHandler *ResourceObject = new ResourceHandler;

    std::vector<Application> apps;
    std::vector<Application> doneApps;
	ConfigHandler ConfigObject(argv, apps, logHandlerObject, MemoryObject, ResourceObject);


    if (ConfigObject.filesRead == true)
    {
        //Start Clock
        initProgramTime();

        std::string simStartString;
        simStartString.append(std::to_string(getTime()));
        simStartString.append(" - Simulator program starting");

        if (ConfigObject.schedulingAlgorithm == "STR")
        {
            simStartString.append(" using ");
            simStartString.append(ConfigObject.schedulingAlgorithm);
            simStartString.append(" sorting");

            //Do the sorting by SJF
            sortSJF(apps);
        }

        else if (ConfigObject.schedulingAlgorithm == "RR")
        {
            simStartString.append(" using ");
            simStartString.append(ConfigObject.schedulingAlgorithm);
            simStartString.append(" sorting");
        }

        else
        {
            std::string errorString = "Error: Invalid Scheduling Algorithm: ";
            errorString.append(ConfigObject.schedulingAlgorithm);

            logHandlerObject->Log(errorString);
            logHandlerObject->Log("Please use one of these: STR, RR");
            logHandlerObject->Log("Using RR...\n");
        }

        logHandlerObject->Log(simStartString);

        //Initialize the apps
        for (unsigned int i = 0; i < apps.size(); i++)
        {
            apps[i].Initialize(ConfigObject.cycleTimeVector);
        }

        //Run the apps
        unsigned int currentAppSize = apps.size();
        while(apps.size() > 0)
        {
            for (unsigned int i = 0; i < currentAppSize; i++)
            {
                if (apps[i].Run(ConfigObject.cycleTimeVector) == 0)
                {
                    doneApps.push_back(apps[i]);
                    apps.erase(apps.begin() + i);
                    currentAppSize--;
                    i--;
                }
            }
        }

        std::cout << "\nWaiting for I/O threads to complete..\n";

        //Wait for I/O threads to complete
        for (unsigned int i = 0; i < doneApps.size(); i++)
        {
            for (unsigned int j = 0; j < doneApps[i].IOthreads.size(); j++)
            {
                pthread_join(doneApps[i].IOthreads[j], NULL);
            }
        }


        std::string simEndString;
        simEndString.append(std::to_string(getTime()));
        simEndString.append(" - Simulator program ending ");
        logHandlerObject->Log(simEndString);

    }

    logHandlerObject->PrintLog();
}




void InitializeGlobals()
{ 
	std::cout.precision(6);
	std::cout.setf( std::ios::fixed);

    configCommandArray = new std::string[ConfigHandler::numConfigs];
    
    configCommandArray[0] = "Processor cycle";
    configCommandArray[1] = "Monitor";
    configCommandArray[2] = "Hard drive";
    configCommandArray[3] = "Printer";
    configCommandArray[4] = "Keyboard";
    configCommandArray[5] = "Memory";
    configCommandArray[6] = "Mouse";
    configCommandArray[7] = "Speaker";
    configCommandArray[8] = "Log:";
    configCommandArray[9] = "Log File Path";
    configCommandArray[10] = "End";
    configCommandArray[11] = "File Path";
    configCommandArray[12] = "System memory";
    configCommandArray[13] = "Memory block size";
    configCommandArray[14] = "Printer quantity";
    configCommandArray[15] = "Hard drive quantity";
    configCommandArray[16] = "CPU Scheduling Code";
    configCommandArray[17] = "Quantum";
}

#endif
