#include "BasicFunctions.h"

static long long programEpoch;


std::string IntToString(int intIn)
{
	std::string intString;

    if (intIn == 0)
    {
        intString.push_back('0');
    }

	while(intIn > 0)
	{
	  intString.insert(0, 1, (intIn % 10) + '0'); 
	                                
	  intIn = intIn / 10;
	}


	return intString;
}




int StringToInt(std::string stringIn)
{
	int integerOutput = 0;

	for (unsigned int i = 0; i < stringIn.length(); i++)
	{
		integerOutput += ( pow(10, (stringIn.length() - i - 1)) * (stringIn[i] - '0') );
	}

	return integerOutput;
}




std::string getAfterColon(std::string stringIn)
{
    std::string stringOut;

    unsigned int i = 0;

	while(i <= stringIn.length())
	{
		if (stringIn[i] == ':')
		{
			i++;
			break;
		}

		i++;
	}

	if (i == stringIn.length())
	{
		return stringOut;
	}

	while(stringIn[i] == 0x20)
	{
		i++;
	}

	int iterator = 0;

	while(i < stringIn.length())
	{
		stringOut.push_back(stringIn[i]);

		i++;
		iterator++;
	}

	return stringOut;
}




bool StringCompare(std::string stringIn, std::string stringBase)
{
	for (int unsigned i = 0; i < stringBase.length(); i++)
	{
		if (stringIn[i] != stringBase[i] && 
			stringIn[i] != stringBase[i] + 32 && 	//if the input is a upper case
			stringIn[i] + 32 != stringBase[i]		//if the intput is a lower case
			)
		{
			return false;
		}
	}

	return true;
}





void initProgramTime()
{
	programEpoch = std::chrono::duration_cast<CHRONO_RESOLUTION>(
	std::chrono::steady_clock::now().time_since_epoch()
	).count();
}




double getTime()
{
	long long currentTime = std::chrono::duration_cast<CHRONO_RESOLUTION>(
	std::chrono::steady_clock::now().time_since_epoch()
	).count();

	double returnTime = (currentTime - programEpoch) / 1000000.0;

	return returnTime;
}




void wait(int timeToWait)
{
    //calculate in microseconds
	timeToWait *= 1000;

	long long waitTime = std::chrono::duration_cast<CHRONO_RESOLUTION>(
		std::chrono::steady_clock::now().time_since_epoch()
		).count();

	long long currentTime = std::chrono::duration_cast<CHRONO_RESOLUTION>(
		std::chrono::steady_clock::now().time_since_epoch()
		).count();


	while(currentTime - waitTime < timeToWait)
	{
		currentTime = std::chrono::duration_cast<CHRONO_RESOLUTION>(
		std::chrono::steady_clock::now().time_since_epoch()
		).count();
	}	
}



