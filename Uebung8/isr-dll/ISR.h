#ifndef __ISR_H__
#define __ISR_H__


#define IOCTL_ISR_INFO	0
#define IOCTL_GET_VALUE	1

typedef struct
{
	DWORD SysIntr;
	GPIOREG* pGPIORegs;
	// other registers or parameters may be mecassary
} ISR_INFO;




#endif