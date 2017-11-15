#include "MemoryHandler.h"


MemoryHandler::MemoryHandler()
{
    system_memory = 0;
    block_size = 0;
    last_block = 1;  //this signifies no memory has been allocated
}

MemoryHandler::~MemoryHandler()
{

}

void MemoryHandler::Initialize()
{
    last_block = 1; //this signifies no memory has been allocated
}

std::string MemoryHandler::AllocateMemory()
{
    unsigned int address; 
    std::string returnString;


    //allocating first block
    if (last_block == 1)
    {
        address = 0;
    }
   
    else
    {
        address = (last_block + block_size) % system_memory;        
    }

    last_block = address;

	int buffersize = 8;
	char hexBuffer[buffersize];

	for (int i = 0; i < buffersize; i++)
	{
		hexBuffer[i] = '\0';
	}

	snprintf(hexBuffer, sizeof(hexBuffer), "%x", address);
	
	for (int i = buffersize - 1; i >= 0; i--)
	{
		if ((unsigned int)hexBuffer[i] < 45)
		{
			returnString.push_back('0');
		}
	}
	
	returnString.append(std::string(hexBuffer));

    return returnString;
}

