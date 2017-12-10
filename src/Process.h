#ifndef PROCESS_H_
#define PROCESS_H_

#include <string>

enum ProcessState
{
	NEW = 0,
	RUNNING = 1,
	WAITING = 2,
	READY = 3,
	TERMINATED = 4
};

struct PCB
{
    int processState;
};

//Valid Codes
enum ProcessCode
{
	S = 0,	//OS
	A = 1,	//Application
	P = 2,	//Process
	I = 3,	//Input
	O = 4,	//Output
	M = 5	//Memory
};

struct Process
{
	ProcessCode processType;
	std::string processName;
	int numCycles;
	int runTime;
	PCB controlBlock;
};

//Convert ProcessCode to Character
char CodeToChar(ProcessCode codeInput);

//Convert Character To ProcessCode
ProcessCode CharToCode(char charInput);

//Valid Code Check
bool isValidProcessCode(char codeToCheck);

#endif
