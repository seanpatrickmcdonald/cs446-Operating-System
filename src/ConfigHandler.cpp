#include "ConfigHandler.h"

ConfigHandler::ConfigHandler()
{
    
}

ConfigHandler::ConfigHandler(char **argv, std::vector<Application> &apps, LogHandler *logHandlerIn, MemoryHandler *MemoryObjectIn, ResourceHandler *ResourceObjectIn)
{
    logHandler = logHandlerIn;
    MemoryObject = MemoryObjectIn;
    ResourceObject = ResourceObjectIn;

	cycleTimeVector.resize(8);

    filesRead = false;

	logHandler->logToFile = false;
	logHandler->logToMonitor = false;
   
    std::vector<std::string> configVector;
  
	std::fstream fileBuffer;
    fileBuffer.open(argv[1], std::ios::in );

    std::string metaFileString = "null";

    if (!fileBuffer)
    {
    	std::string errorString;

	  	errorString.append("Invalid Config File: ");
	  	errorString.append(argv[1]);
	  	errorString.append("\nExiting..");

	  	std::cout << errorString << std::endl;        
    }

    else
    {
        filesRead = true;

        std::string tempString;
        //read in all lines to vector
        while(!fileBuffer.eof())
        {
            std::getline(fileBuffer, tempString);
            configVector.push_back(tempString);
        }
	
	    fileBuffer.close();

        for (unsigned int i = 0; i < configVector.size(); i++)
        {
            if (StringCompare(configVector[i], configCommandArray[LogType]))
	    	{
	    		std::string logType = "Log to Both";
	    		std::string logToFileString = "Log To File";
	    		std::string logToMonString = "Log To Monitor";

	    		std::string aString = getAfterColon(configVector[i]);

	    		if (StringCompare(logType, aString) || StringCompare(logToMonString, aString))
	    		{
	    			logHandler->logToMonitor = true;
	    		}

                if (StringCompare(logType, aString) || StringCompare(logToFileString, aString))
                {
				    logHandler->logToFile = true;
                }

	    		continue;
	    	}            
            
	    	//Log Path Processing
	    	else if (StringCompare(configVector[i], configCommandArray[LogFilePath]))
	    	{
				logHandler->logOutputPath = getAfterColon(configVector[i]);

	    		continue;    		
	    	}

            //System Memory
	    	else if (StringCompare(configVector[i], configCommandArray[SystemMemory]))
	    	{
	    		MemoryObject->system_memory = StringToInt(getAfterColon(configVector[i]));
	    	}

            //Memory Block Size
	    	else if (StringCompare(configVector[i], configCommandArray[MemoryBlock]))
	    	{
	    		MemoryObject->block_size = StringToInt(getAfterColon(configVector[i]));
	    	}

            //Printer Quantity
	    	else if (StringCompare(configVector[i], configCommandArray[PrinterQuan]))
	    	{
	    		ResourceObject->num_printers = StringToInt(getAfterColon(configVector[i]));
	    	}

            //Hard Drive Quantity
	    	else if (StringCompare(configVector[i], configCommandArray[HardDriveQuan]))
	    	{
	    		ResourceObject->num_hdds = StringToInt(getAfterColon(configVector[i]));
	    	} 
 
            //Meta Data File Path processing
            else if (StringCompare(configVector[i], configCommandArray[MetaFilePath]))
	    	{
				metaFileString = getAfterColon(configVector[i]);
            }
 
            //Scheduling Type
            else if (StringCompare(configVector[i], configCommandArray[Scheduling]))
	    	{
				schedulingAlgorithm = getAfterColon(configVector[i]);
            }

            //Quantum Number
            else if (StringCompare(configVector[i], configCommandArray[Quantum])
            	|| StringCompare(configVector[i], "Processor Quantum"))
            {
            	Application::quantum = StringToInt(getAfterColon(configVector[i]));
            }

	    	//Otherwise, we might have a Configuration Cyle Parameter
	    	else
	    	{
	    		for (int j = Processor; j <= Speaker; j++)
	    		{
	    			if (StringCompare(configVector[i], configCommandArray[j]))
	    			{
	    				std::string processTimeString = getAfterColon(configVector[i]);
	    				int cycles = StringToInt(processTimeString);

	    				cycleTimeVector[j] = cycles;	 				
	    			}
	    		}
	    	}  
        }
    }

	filesRead = ReadMetaFile(apps, metaFileString);

    MemoryObject->Initialize();
    ResourceObject->Initialize();
}




ConfigHandler::~ConfigHandler()
{

}



/*
void ConfigHandler::LogConfigFileData(Application &app)
{
	logHandler->Log(" ");
	logHandler->Log("Configuration File Data");

	for (unsigned int i = 0; i < cycleTimeVector.size(); i++)
	{
		std::string outputString;

		outputString.append(configCommandArray[i]);
		outputString.append(" = ");
		outputString.append(IntToString(cycleTimeVector[i]));
		outputString.append(" ms/cycle");


		logHandler->Log(outputString);
	}

	std::string outputString;

	outputString.append("Logged to: ");
	
	if (logHandler->logToMonitor)
	{
		outputString.append("monitor ");
	}

	if (logHandler->logToMonitor && logHandler->logToFile)
	{
		outputString.append("and ");
	}

	if (logHandler->logToFile)
	{
		outputString.append(logHandler->logOutputPath);
	}

	logHandler->Log(outputString);
}
*/




//valid commands based on code
void ConfigHandler::metaInitialize()
{
	descriptorArray[S].push_back("start");
	descriptorArray[S].push_back("end");

	descriptorArray[A].push_back("start");
	descriptorArray[A].push_back("end");

	descriptorArray[P].push_back("run");

	descriptorArray[I].push_back("hard drive");
	descriptorArray[I].push_back("keyboard");
	descriptorArray[I].push_back("mouse");

	descriptorArray[O].push_back("hard drive");
	descriptorArray[O].push_back("monitor");
	descriptorArray[O].push_back("speaker");
	descriptorArray[O].push_back("printer");

	descriptorArray[M].push_back("block");
	descriptorArray[M].push_back("allocate");
}

//Function to make sure an operation given by file is valid for said code
bool ConfigHandler::CheckValidOperation(std::string operationTest, ProcessCode codeTest)
{
	for (unsigned int j = 0; j < descriptorArray[codeTest].size(); j++)
	{
		if (StringCompare(operationTest, descriptorArray[codeTest][j]))
		{
			return true;
		}
	}

	return false;
}

bool ConfigHandler::ReadMetaFile(std::vector<Application> &apps, std::string &metaFileString)
{
	std::string tempString;

 	std::fstream fileBuffer;
	fileBuffer.open(metaFileString.c_str(), std::ios::in);
    std::vector<std::string> metaFileVector;

	if (fileBuffer)
	{
		while (!fileBuffer.eof())
		{

	        std::getline(fileBuffer, tempString);
	        metaFileVector.push_back(tempString);  	
		}
	}

	else
	{
		std::cout << "Invalid Meta File: " << metaFileString << std::endl;
		std::cout << "Exiting.." << std::endl;

		return false;
	}

    
	metaInitialize();

	Application *current_app;
	int processNumber = 1;
	unsigned int num_io_tasks = 0;
	unsigned int num_tasks = 0;

    for (unsigned int line_num = 0; line_num < metaFileVector.size(); line_num++)
	{
        std::string current_line = metaFileVector[line_num];


		if (StringCompare(current_line, "Start"))
		{
			continue;
		}

		if (StringCompare(current_line, "End"))
		{
			break;
		}

		for (unsigned int i = 0; i < current_line.length(); i++)
	    {
		    if ((current_line[i] == ';' || i == 0) )
		    {
			    while(current_line[i] == ' ' || current_line[i] == ';')
			    {
				    i++;
			    }

			    if(i < current_line.length() && !isValidProcessCode(current_line[i]))
			    {
				    //carriage return issues, gotta work on this
				    if (int(current_line[i]) == 13)
				    {
					    break;
				    }

				    std::string errorString = "\nError - Invalid ProcessCode [";
				    errorString.push_back(int(current_line[i]));
				    errorString.append("] at Line Number: ");
				    errorString.append(IntToString(line_num));

				    logHandler->Log(errorString);
			    }

			    else if (i < current_line.length())
			    {
				    ProcessCode processCode = CharToCode(current_line[i]);

				    i++;

				    std::string processString;



				    if (current_line[i] == '(')
				    {
					    i++;

					    while(current_line[i] != ')')
					    {
						    processString.push_back(current_line[i]);
						    i++;
					    }

					    bool validOperation = CheckValidOperation(processString, processCode);

					    if (!validOperation)
					    {
						    std::string errorString;
						    errorString.append("\nError - Invalid Operation (");
						    errorString.append(processString);
						    errorString.append(") at Line Number: ");
						    errorString.append(IntToString(line_num));

						    errorString.append("\nList of Valid Operations for ProcessCode [");
						    errorString.push_back(CodeToChar(processCode));
						    errorString.append("]\n");

						    for (unsigned int i = 0; i < descriptorArray[processCode].size(); i++)
						    {
							    errorString.append(descriptorArray[processCode][i]);
							    errorString.push_back('\n');
						    }

						    logHandler->Log(errorString);

						    continue;
					    }

					    //Get our current config Command
						configCommand currentConfig = ConfigHandler::StringToConfigCommand(processString);

					    i++;

					    std::string intString;

					    while(current_line[i] >='0' && current_line[i] <= '9')
					    {
						    intString.push_back(current_line[i]);

						    i++;
					    }

					    int processCycles = StringToInt(intString);

                        if (processCode == A)
                        {
                        	if (processString == "start")
                        	{
                        		current_app = new Application(processNumber, logHandler, MemoryObject, ResourceObject);

                        		processNumber++;
                        	}

                        	if (processString == "end")
                        	{
								current_app->num_tasks = num_tasks;
								current_app->num_io_tasks = num_io_tasks;
								
								num_tasks = 0;
								num_io_tasks = 0;

                        		apps.push_back(*current_app);

                        		current_app = nullptr;
                        	}
                        }

                        int runTime = processCycles * cycleTimeVector[currentConfig];
                         Process dummyProcess = {
						    .processType = processCode,
						    .processName = processString,
						    .numCycles = processCycles,
						    .runTime = runTime
					    };

					    if (processCode != S && processCode != A && current_app != nullptr)
					    {
					    	current_app->processVector.push_back(dummyProcess);

					    	num_tasks++;
					    }

					    if (processCode == I || processCode == O)
					    {
					    	num_io_tasks++;
					    }

						if (processCode == S && processString == "end" && current_app != nullptr)
						{
							logHandler->Log("Warning: A(start) without matching A(end), and end of simulation found.");
							logHandler->Log("Don't worry, I'll take care of that for you..\n");

							current_app->num_tasks = num_tasks;
							current_app->num_io_tasks = num_io_tasks;

                    		apps.push_back(*current_app);

                    		current_app = nullptr;
						}

					    i--;
				    } //if '('
			    }
		    }// if ((current_line[i] == ';' || i == 0) )
	    }
	}

    return true;
}




configCommand ConfigHandler::StringToConfigCommand(std::string stringIn)
{
	for (int i = Processor; i <= Speaker; i++)
	{
		bool commandFound = StringCompare(stringIn, configCommandArray[i]);

		if (commandFound)
		{
			switch(i)
			{
				case Processor: return Processor;
				case Monitor: return Monitor;
				case HardDrive: return HardDrive;
				case Printer: return Printer;
				case Keyboard: return Keyboard;
				case Memory: return Memory;
				case Mouse: return Mouse;
				case Speaker: return Speaker;
			}

			break;
		}
	}

	return Processor;
}


