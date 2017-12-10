#ifndef CONFIG_HANDLER_H_
#define CONFIG_HANDLER_H_

#include "SeanBasics.h"
#include "Application.h"
#include "BasicFunctions.h"
#include "Process.h"

class Application;

enum configCommand
{	
	Processor = 0,
	Monitor = 1,
	HardDrive = 2,
	Printer = 3,
	Keyboard = 4,
	Memory = 5,
	Mouse = 6,
	Speaker = 7,
	LogType = 8,
	LogFilePath = 9,
	EndCommand = 10,	
	MetaFilePath = 11,
	SystemMemory = 12,
    MemoryBlock = 13, 
    PrinterQuan = 14,
    HardDriveQuan = 15,
    Scheduling = 16,
    Quantum = 17
};

class ConfigHandler
{
public:
    ConfigHandler();
    ConfigHandler(char **argv, std::vector<Application> &apps, LogHandler *, MemoryHandler *, ResourceHandler *);
    ~ConfigHandler();

    static configCommand StringToConfigCommand(std::string stringIn);
    //void LogConfigFileData(Application &app);
    bool ReadMetaFile(std::vector<Application> &apps, std::string &metaFileString);
    bool CheckValidOperation(std::string operationTest, ProcessCode codeTest);
    void metaInitialize(); 
    void buildCommandVector(std::string stringIn, int lineNumber, Application &app);

    bool filesRead;
    static const unsigned int numConfigs = 18;
    std::vector<std::string> descriptorArray[6];

    std::vector<unsigned int> cycleTimeVector;

    std::string schedulingAlgorithm;

    LogHandler *logHandler;
    MemoryHandler *MemoryObject;
    ResourceHandler *ResourceObject;
};



#endif
