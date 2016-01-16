#include "Types.h"

void printstring(int iX, int iY, const char *string);

void main()
{
	printstring(0, 10, "Switch To IA-32e Mode Success!");
	printstring(0, 12, "IA-32e C Language Kernel Start!");
	printstring(0, 13, "IA-32e C Language Kernel Start!");
	printstring(0, 14, "IA-32e C Language Kernel Start!");
	printstring(0, 15, "IA-32e C Language Kernel Start!");
	while(1);
}

void printstring(int iX, int iY, const char *String)
{
	CHARACTER *pstScreen = (CHARACTER*)0xB8000;
	int i;

	pstScreen += (iY * 80) + iX;

	for(i = 0; String[i] != 0; i++)
	{
		pstScreen[i].Charactor = String[i];
	}
}

