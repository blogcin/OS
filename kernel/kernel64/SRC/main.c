#include "Types.h"
#include "keyboard.h"

void printstring(int iX, int iY, const char *string);

void main()
{
	unsigned char temp;
	unsigned char flags;
	char keyTemp[2] = { 0, };
	int i = 0;

	printstring(0, 10, "Switch To IA-32e Mode Success!");
	printstring(0, 11, "IA-32e C Language Kernel Start!");

	printstring(0, 12, "Keyboard Activate ...");
	if(activateKeyboard())
	{
		printstring(22, 12, "Pass");
		changeKeyboardLED(0, 0, 0);
	}
	else
	{
		printstring(22, 12, "Pass");
		while(1);
	}

	while(1)
	{
		if(isOutputBufferFull())
		{
			temp = getKeyboardScanCode();
		
			if(convertScanCodeToASCIICode(temp, &(keyTemp[0]), &flags))
			{
				if(flags & KEY_FLAGS_DOWN)
				{
					printstring(i++, 13, keyTemp);
				}
			}
		}
	}
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

