#ifndef __UTIL_H__
#define __UTIL_H__

void memSet(void *destination, unsigned char data, int size);
int memCpy(void *destination, const void *source, int size);
int memCmp(const void *destination, const void *source, int size);
#endif
