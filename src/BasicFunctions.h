#ifndef BASIC_FUNCTIONS_H
#define BASIC_FUNCTIONS_H

#include "SeanBasics.h"

#define CHRONO_RESOLUTION std::chrono::microseconds


/*************************************************************

	IntToString

**************************************************************
Take an int, make a string
*/
std::string IntToString(int intIn);


/*************************************************************

	StringToInt

**************************************************************
Take a string, make an int
*/
int StringToInt(std::string stringIn);


/*************************************************************

	getAfterColon

**************************************************************
Get stuff after colon
*/
std::string getAfterColon(std::string stringIn);


/*************************************************************

	StringCompare

**************************************************************
Custom String Test
*/
bool StringCompare(std::string stringIn, std::string stringBase);    








/*************************************************************

	getTime

**************************************************************
returns current time since programEpoch
*/
double getTime();


/*************************************************************

	initProgramTime

**************************************************************
Sets programEpoch
*/
void initProgramTime();


/*************************************************************

	wait

**************************************************************
int timeToWait - Time to wait in milliseconds
*/
void wait(int timeToWait);


#endif
