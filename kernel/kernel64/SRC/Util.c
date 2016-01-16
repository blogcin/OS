
void memSet(void *destination, unsigned char data, int size)
{
	int i;
	for(i = 0; i < size; i++)
	{
		((char *)destination)[i] = data;
	}
}
void memCpy(void *destination, const void *source, int size)
{
	int i;
	for(i = 0; i < size; i++)
	{
		((char *)destination)[i] = ((char *)source)[i];
	}

	return size;
}
int memCmp(const void *destination, const void *source, int size)
{
	int i;
	char temp;
	
	for(i = 0; i < size; i++)
	{
		temp = ((char *)destination)[i] - ((char *)source)[i];
		if(temp != 0)
			return (int)temp;
	}
	return 0;
}

