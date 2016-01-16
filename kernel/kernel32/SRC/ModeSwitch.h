#ifndef __MODESWITCH_H__
#define __MODESWITCH_H__

void ReadCPUID(unsigned int dwEAX, unsigned int *pdwEAX, unsigned int *pdwEBX, unsigned int *pdwECX, unsigned int *pdwEDX);
	
void SwitchKernelto64();
#endif
