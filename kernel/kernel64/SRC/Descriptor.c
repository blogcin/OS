#include "Descriptor.h"
#include "Util.h"

void initGDTTableAndTSS()
{
	GDTR* gdtr;
	GDTENTRY8* Entry;
	TSSSEGMENT* TSS;
	int i;

	gdtr = (GDTR*)GDTR_STARTADDRESS;
	Entry = (GDTENTRY8*)(GDTR_STARTADDRESS + sizeof(GDTR));
	gdtr->limit = GDT_TABLESIZE - 1;
	gdtr->baseAddress = (unsigned long)Entry;
	TSS = (TSSSEGMENT*) ((unsigned long)Entry + GDT_TABLESIZE);

	setGDTEntry8(&(Entry[0]), 0, 0, 0, 0, 0);

	setGDTEntry8(&(Entry[1]), 0, 0xFFFFF, GDT_FLAGS_UPPER_CODE,
		GDT_FLAGS_LOWER_KERNELCODE, GDT_TYPE_CODE);

	setGDTEntry8(&(Entry[2]), 0, 0xFFFFF, GDT_FLAGS_UPPER_DATA,
		GDT_FLAGS_LOWER_KERNELDATA, GDT_TYPE_DATA);

	setGDTEntry16((GDTENTRY16*)&(Entry[3]), (unsigned long)TSS,
		sizeof(TSSSEGMENT) - 1, GDT_FLAGS_UPPER_TSS, GDT_FLAGS_LOWER_TSS,
		GDT_TYPE_TSS);

	initTSSSegment(TSS);
}

void setGDTEntry8(GDTENTRY8* Entry, unsigned int baseAddress, unsigned int limit, 
		unsigned char upperFlags, unsigned char lowerFlags, 
		unsigned char type)
{
	Entry->lowerLimit = limit & 0xFFFF;
	Entry->lowerBaseAddress = baseAddress & 0xFFFF;
	Entry->upperBaseAddress1 = (baseAddress >> 16) & 0xFF;
	Entry->typeAndLowerFlag = lowerFlags | type;
	Entry->upperLimitAndUpperFlag = ((limit >> 16) & 0xFF) | upperFlags;
	Entry->upperBaseAddress2 = (baseAddress >> 24) & 0xFF;
}

void setGDTEntry16(GDTENTRY16* entry, unsigned long baseAddress, unsigned int limit,
        unsigned char upperFlags, unsigned char lowerFlags, unsigned char type)
{
	entry->lowerLimit = limit & 0xFFFF;
	entry->lowerBaseAddress = baseAddress & 0xFFFF;
	entry->middleBaseAddress1 = ( baseAddress >> 16 ) & 0xFF;
	entry->typeAndLowerFlag = lowerFlags | type;
	entry->upperLimitAndUpperFlag = ( ( limit >> 16 ) & 0xFF ) | 
        	upperFlags;
    	entry->middleBaseAddress2 = ( baseAddress >> 24 ) & 0xFF;
    	entry->upperBaseAddress = baseAddress >> 32;
    	entry->reserved = 0;
}

void initTSSSegment(TSSSEGMENT* TSS)
{
    	memSet(TSS, 0, sizeof(TSSSEGMENT) );
    	TSS->IST[ 0 ] = IST_STARTADDRESS + IST_SIZE;
    	TSS->ioMapBaseAddress = 0xFFFF;
}

void initIDTTables()
{
    	IDTR* idtr;
    	IDTENTRY* entry;
    	int i;
        
    	idtr = ( IDTR* ) IDTR_STARTADDRESS;
    	entry = ( IDTENTRY* ) ( IDTR_STARTADDRESS + sizeof( IDTR ) );
    	idtr->baseAddress = (unsigned long)entry;
    	idtr->limit = IDT_TABLESIZE - 1;

    	for( i = 0 ; i < IDT_MAXENTRYCOUNT ; i++ )
    	{
        	setIDTEntry( &( entry[ i ] ), dummyHandler, 0x08, IDT_FLAGS_IST1, 
            	IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT );
    	}
}

void setIDTEntry(IDTENTRY* entry, void* handler, unsigned short selector, 
        unsigned char ist, unsigned char flags, unsigned char type)
{
    	entry->lowerBaseAddress = (unsigned long) handler & 0xFFFF;
    	entry->segmentSelector = selector;
    	entry->IST = ist & 0x3;
    	entry->typeAndFlags = type | flags;
    	entry->middleBaseAddress = ((unsigned long) handler >> 16 ) & 0xFFFF;
    	entry->upperBaseAddress = (unsigned long) handler >> 32;
    	entry->reserved = 0;
}

void dummyHandler()
{
    printstring( 0, 0, "====================================================" );
    printstring( 0, 1, "          Dummy Interrupt Handler Execute~!!!       " );
    printstring( 0, 2, "           Interrupt or Exception Occur~!!!!        " );
    printstring( 0, 3, "====================================================" );

    while( 1 ) ;
}
