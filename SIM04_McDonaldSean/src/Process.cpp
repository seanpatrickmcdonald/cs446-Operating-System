#include "Process.h"


//Convert ProcessCode to Character
char CodeToChar(ProcessCode codeInput)
{
	switch(codeInput)
	{
		case S: return 'S';
		case A: return 'A';
		case P: return 'P';
		case I: return 'I';
		case O: return 'O';
		case M: return 'M';		
	}	

	return '\0';
}

//Convert Character To ProcessCode
ProcessCode CharToCode(char charInput)
{
	switch(charInput)
	{
		case 'S': return S;
		case 'A': return A;
		case 'P': return P;
		case 'I': return I;
		case 'O': return O;
		case 'M': return M;		
	}

	return S;
}

//Valid Code Check
bool isValidProcessCode(char codeToCheck)
{
	bool returnBool = false;

	switch(codeToCheck)
	{
		case 'S': returnBool = true;
		case 'A': returnBool = true;
		case 'P': returnBool = true;
		case 'I': returnBool = true;
		case 'O': returnBool = true;
		case 'M': returnBool = true;		
	}

	return returnBool;
}
