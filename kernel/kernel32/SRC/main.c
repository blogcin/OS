#include "Types.h"
#include "page.h"
#include "ModeSwitch.h"

void printstring(int iX, int iY, const char *String);
unsigned char isMemoryEnough();
unsigned char initializeMemoryArea();
void copyKerneltoMemory();

void main(void)
{
	unsigned int i;
	unsigned int dwEAX, dwEBX, dwECX, dwEDX;
	char vendorString[13] = {0};

	printstring(0, 3, "C Language Kernel Start!");
	printstring(0, 4, "64bit Minimum Memory Check....");
	
	if(isMemoryEnough())
	{
		printstring(33, 4, "Failed");
		//printstring(0, 5, "You don't have enough memory");
		while(1); 
	}
	else
		printstring(33, 4, "Pass");

	printstring(0, 5, "Initializing IA-32e Area....");
	if(initializeMemoryArea())
	{
		printstring(35, 5, "Failed");
	}
	else
		printstring(35, 5, "Pass");

	printstring(0, 6, "IA-32e Page Tables Init....");
	
	InitPageTables();

	printstring(30, 6, "Pass");
	
	ReadCPUID(0x00, &dwEAX, &dwEBX, &dwECX, &dwEDX);

	*(unsigned int*)vendorString = dwEBX;
	*((unsigned int*)vendorString+1) = dwEDX;
	*((unsigned int*)vendorString+2) = dwECX;

	printstring(0, 7, "Processor Vendor...");
	printstring(21, 7, vendorString);

	ReadCPUID(0x80000001, &dwEAX, &dwEBX, &dwECX, &dwEDX);
	printstring(0, 8, "64bit Support Check....");
	if(dwEDX & ( 1 << 29))
	{
		printstring(35, 8, "Pass");
	}
	else
	{
		printstring(35, 8, "Fail");
		while(1);
	}
	
	printstring(0, 9, "Copy IA-32e Kernel to Memory...");
	
	copyKerneltoMemory();
	printstring(33, 9, "Pass");

	printstring(0, 10, "Boot to IA-32e Kernel");
	SwitchKernelto64();
	printstring(0, 11, "bOOOOT!!");
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

unsigned char initializeMemoryArea()
{
	unsigned int *CurrentAddress;

	CurrentAddress = (unsigned int*) 0x100000;

	while ((unsigned int) CurrentAddress < 0x600000 )
	{
		*CurrentAddress = 0x00;

		if (*CurrentAddress != 0 )
		{
			return 1;
		}

		CurrentAddress += 1;
	}

	return 0;
}

unsigned char isMemoryEnough()
{
	unsigned int *CurrentAddress;

	CurrentAddress = (unsigned int*) 0x100000;

	while ((unsigned int) CurrentAddress < 0x4000000)
	{
		*CurrentAddress = 0x12345678;

		if (*CurrentAddress != 0x12345678)
		{
			return 1;
		}

		CurrentAddress += (0x100000/4);
	}

	return 0;
}

void copyKerneltoMemory()
{
	unsigned short Kernel32SectorCount;
	unsigned short TotalKernelSectorCount;
	unsigned int *SourceAddress;
	unsigned int *DestinationAddress;
	int i;
	
	TotalKernelSectorCount = *((unsigned short*)0x7C05);
	Kernel32SectorCount = *((unsigned short*)0x7C07);
	
	SourceAddress = (unsigned int*)(0x10000 + (Kernel32SectorCount*512));
	DestinationAddress = ( unsigned int * ) 0x200000;

	for ( i = 0 ; i < 512 * ( TotalKernelSectorCount - Kernel32SectorCount ) / 4; i++ )
	{
		*DestinationAddress = *SourceAddress;
		DestinationAddress += 1;
		SourceAddress += 1;
	}
}
