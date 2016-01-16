#include "Types.h"

void printstring(int iX, int iY, const char *String);

void main(void)
{
	printstring(0, 5, "C Language Kernel Start!");
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
