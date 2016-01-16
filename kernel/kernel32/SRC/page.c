#include "page.h"

void InitPageTables()
{
	PML4TENTRY *PML4TEntry;
	PDPTENTRY *PDPTEntry;
	PDENTRY *PDEntry;
	unsigned int MappingAddress;
	int i;

	PML4TEntry = (PML4TENTRY*) 0x100000;
	setPageEntryData(&(PML4TEntry[0]), 0x00, 0x101000, PAGE_FLAGS_DEFAULT, 0);

	for(i = 1; i < PAGE_MAXENTRYCOUNT; i++)
	{
		setPageEntryData(&(PML4TEntry[i]), 0, 0, 0, 0);
	}

	PDPTEntry = (PDPTENTRY*) 0x101000;
	for(i = 0; i < 64; i++)
	{
		setPageEntryData(&(PDPTEntry[i]), 0, 0x102000 + ( i * PAGE_TABLESIZE), PAGE_FLAGS_DEFAULT, 0);
	}

	for(i = 64; i < PAGE_MAXENTRYCOUNT; i++)
	{
		setPageEntryData(&(PDPTEntry[i]), 0, 0, 0, 0);
	}

	PDEntry = (PDENTRY*) 0x102000;
	MappingAddress = 0;
	
	for(i = 0; i < PAGE_MAXENTRYCOUNT * 64; i++)
	{
		setPageEntryData(&(PDEntry[i]), (i * (PAGE_DEFAULTSIZE >> 20)) >> 12, MappingAddress, PAGE_FLAGS_DEFAULT | PAGE_FLAGS_PS, 0);
		MappingAddress += PAGE_DEFAULTSIZE;
	}	
}


void setPageEntryData(PTENTRY* Entry, unsigned int UpperBaseAddress, 
	unsigned int LowerBaseAddress, unsigned int LowerFlags, 
	unsigned int UpperFlags)
{
	Entry->AttributeAndLowerBaseAddress = LowerBaseAddress | LowerFlags;
	Entry->UpperBaseAddressAndEXB = (UpperBaseAddress & 0xFF) | UpperFlags;
}
